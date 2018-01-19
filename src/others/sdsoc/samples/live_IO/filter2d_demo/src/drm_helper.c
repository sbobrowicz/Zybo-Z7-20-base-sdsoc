/*
 * drm_helper.c
 *
 *  Created on: Nov 8, 2017
 *      Author: digilent
 */
#include <drm_helper.h>
#include "platform.h"

/*
 *  Allocates space for a 'drm_control' struct and returns a pointer to it
 *  Sets the CRTC to the resolution requested
 *  Creates 4 dumb buffers with the requested resolutions and maps them into virtual memory
 *  populates the drm_control structure with all the necessary information
 *  to control linking the different dumb buffers to the CRTC
 *  Initially, dumb buffer 1 is mapped to the CRTC
 *
 *  returns 0 on success
 *  returns -1 on failure
 */
int drmControlInit(char * port, struct drm_mode_modeinfo * mode, drm_cntrl * control ) {
    int dri_fd;
    if(openPort(port, &dri_fd) < 0) {
        fprintf(stderr, "Failed to open the file specified by 'port' %m\n");
        return FAILURE;
    }

    // Populate fields of the main data structure
    //control->current = *mode;    // Set the current mode to the one passed by the user
    control->current_fb = 0;        // Initially link FB0 to the CRTC
    control->dri_fd = dri_fd;

    /* Take control of the DRM MASTER */
    if(ioctl(control->dri_fd, DRM_IOCTL_SET_MASTER, 0) < 0) {
        fprintf(stderr, "%s         ", strerror(errno));
        fprintf(stderr, "Unable to Set Master\n");
        return FAILURE;
    }

    /*****************************************************************/
    /* Get Resource Statistics */
    /*****************************************************************/

    memset(&control->mode_card, 0, sizeof(struct drm_mode_card_res));

    if(ioctl(control->dri_fd, DRM_IOCTL_MODE_GETRESOURCES, &control->mode_card) < 0) {
        fprintf(stderr, "%d %s         ", errno, strerror(errno));
        fprintf(stderr, "Unable to get resources -- 1st pass\n");
        return FAILURE;
    }

    /* These fields will be populated by the next ioctl call */
    memset(control->res_fb_buf, 0, sizeof(uint64_t)*NUM_MAX_RESOURCE_IDS);
    memset(control->res_crtc_buf, 0, sizeof(uint64_t)*NUM_MAX_RESOURCE_IDS);
    memset(control->res_conn_buf, 0, sizeof(uint64_t)*NUM_MAX_RESOURCE_IDS);
    memset(control->res_enc_buf, 0, sizeof(uint64_t)*NUM_MAX_RESOURCE_IDS);
    control->mode_card.fb_id_ptr = control->res_fb_buf;
    control->mode_card.crtc_id_ptr = control->res_crtc_buf;
    control->mode_card.connector_id_ptr = control->res_conn_buf;
    control->mode_card.encoder_id_ptr = control->res_enc_buf;

    if(ioctl(control->dri_fd, DRM_IOCTL_MODE_GETRESOURCES, &control->mode_card) < 0) {
        fprintf(stderr, "%d %s         ", errno, strerror(errno));
        fprintf(stderr, "Unable to get resources -- 2nd pass\n");
        return FAILURE;
    }
    /*****************************************************************/


    /***************************************************************
    *   Get information about the connector and available modes
    ***************************************************************/

    memset(&control->connector, 0, sizeof(struct drm_mode_get_connector));

    memset(control->conn_prop_buf, 0, sizeof(uint64_t)*NUM_MAX_MODES);
    memset(control->conn_propval_buf, 0, sizeof(uint64_t)*NUM_MAX_MODES);
    memset(control->conn_enc_buf, 0, sizeof(uint64_t)*NUM_MAX_MODES);
    memset(control->mode_buffer, 0, sizeof(struct drm_mode_get_connector)*NUM_MAX_MODES);

    control->connector.connector_id = control->res_conn_buf[0];
    if(ioctl(dri_fd, DRM_IOCTL_MODE_GETCONNECTOR, &control->connector) < 0) {
        fprintf(stderr, "%d %s         ", errno, strerror(errno));
        fprintf(stderr, "Unable to get connector -- 1st pass\n");
        return FAILURE;
    }

    control->connector.modes_ptr = (uint64_t) control->mode_buffer;
    control->connector.props_ptr = (uint64_t) control->conn_prop_buf;
    control->connector.prop_values_ptr = (uint64_t) control->conn_propval_buf;
    control->connector.encoders_ptr = (uint64_t) control->conn_enc_buf;
    if(ioctl(dri_fd, DRM_IOCTL_MODE_GETCONNECTOR, &control->connector) < 0) {
        fprintf(stderr, "%d %s         ", errno, strerror(errno));
        fprintf(stderr, "Unable to get connector -- 2nd pass\n");
        return FAILURE;
    }

    /* Check if this connector is currently connected */
    if(control->connector.count_encoders < 1 || control->connector.count_modes < 1 || !(control->connector.encoder_id)
            || !(control->connector.connection)) {
                fprintf(stderr, "Connector Not Connected\n");
                return FAILURE;
    }


    /***************************************************************
    *   Assign desired mode
    ***************************************************************/

    //Currently this overrides the user selected mode and just uses the default mode for the monitor.
    //This should be modified to use the user mode, and maybe check it is valid. sjb

    control->current = control->mode_buffer[0];

    /***************************************************************
    *   Create FOUR dumb buffers
    ***************************************************************/
    for(int i = 0; i < NUM_DUMB_BUFFERS; i++) {
        /* Allocate all of the memory for the frame buffer structs, and assign
            the pointers into our drm_cntrl struct                              */

        memset(&(control->create_dumb[i]), 0, sizeof(struct drm_mode_create_dumb));
        memset(&(control->map_dumb[i]), 0, sizeof(struct drm_mode_map_dumb));
        memset(&(control->cmd_dumb[i]), 0, sizeof(struct drm_mode_fb_cmd));

        control->create_dumb[i].width = INPUT_FB_MAX_W;
        control->create_dumb[i].height = INPUT_FB_MAX_H;
        control->create_dumb[i].bpp = INPUT_FB_BPP*8;
        control->create_dumb[i].flags = 0;
        control->create_dumb[i].pitch = 0;
        control->create_dumb[i].size = 0;
        control->create_dumb[i].handle = 0;
        if(ioctl(control->dri_fd, DRM_IOCTL_MODE_CREATE_DUMB, &(control->create_dumb[i])) < 0) {
            fprintf(stderr, "%d %s         ", errno, strerror(errno));
            fprintf(stderr, "Unable to create dumb buffer\n");
            return FAILURE;
        }

        control->cmd_dumb[i].width = control->create_dumb[i].width;
        control->cmd_dumb[i].height = control->create_dumb[i].height;
        control->cmd_dumb[i].bpp = control->create_dumb[i].bpp;
        control->cmd_dumb[i].pitch = control->create_dumb[i].pitch;
        control->cmd_dumb[i].depth = 32;
        control->cmd_dumb[i].handle = control->create_dumb[i].handle;
        if(ioctl(control->dri_fd, DRM_IOCTL_MODE_ADDFB, &(control->cmd_dumb[i])) < 0) {
            fprintf(stderr, "%d %s         ", errno, strerror(errno));
            fprintf(stderr, "Unable add dumb buffer as FB\n");
            return FAILURE;
        }

        control->map_dumb[i].handle = control->create_dumb[i].handle;
        if(ioctl(control->dri_fd, DRM_IOCTL_MODE_MAP_DUMB, &(control->map_dumb[i])) < 0) {
            fprintf(stderr, "%d %s         ", errno, strerror(errno));
            fprintf(stderr, "Unable to map dumb buffer\n");
            return FAILURE;
        }

        control->fbMem[i] = mmap(NULL, control->create_dumb[i].size, PROT_READ | PROT_WRITE, MAP_SHARED, control->dri_fd, control->map_dumb[i].offset);
        if(control->fbMem[i] == NULL || control->fbMem[i] == -1) {
            fprintf(stderr, "%d %s         ", errno, strerror(errno));
            fprintf(stderr, "Failed to mmap the FB memory\n");
            return FAILURE;
        }
        //memset(control->fbMem[i], 0xFF, control->create_dumb[i].size);
    }

    /***************************************************************
    *   Handle Encoder and CRTC
    ***************************************************************/

    memset(&control->enc, 0, sizeof(struct drm_mode_get_encoder));

    control->enc.encoder_id = control->connector.encoder_id;
    if(ioctl(control->dri_fd, DRM_IOCTL_MODE_GETENCODER, &control->enc) < 0) {
        fprintf(stderr, "%d %s         ", errno, strerror(errno));
        fprintf(stderr, "Failed to get the encoder\n");
        return FAILURE;
    }


    memset(&control->crtc, 0, sizeof(struct drm_mode_crtc));

    control->crtc.crtc_id = control->enc.crtc_id;
    if(ioctl(control->dri_fd, DRM_IOCTL_MODE_GETCRTC, &control->crtc) < 0) {
        fprintf(stderr, "%d %s         ", errno, strerror(errno));
        fprintf(stderr, "Failed to get the CRTC\n");
        return FAILURE;
    }

    control->crtc.fb_id = control->cmd_dumb[control->current_fb].fb_id;
    control->crtc.set_connectors_ptr = (uint64_t) control->res_conn_buf;
    control->crtc.count_connectors = 1;

    control->crtc.mode = control->current; // Set to the requested mode
    control->crtc.mode_valid = 1;

    if(ioctl(control->dri_fd, DRM_IOCTL_MODE_SETCRTC, &control->crtc) < 0) {
        fprintf(stderr, "%d %s         ", errno, strerror(errno));
        fprintf(stderr, "Failed to set the CRTC\n");
        return FAILURE;
    }

    return SUCCESS;
}

/*
 *  Flips a new frame buffer onto the CRTC after the most
 *  recent vertical sync.
 *  The frame buffer to flip to is reference by int fb_num
 *  and represents the index of the frame buffer to use
 *
 *  returns 0 on success
 *  returns -1 on failure
 */
int mapFBtoCRTC(drm_cntrl * control, int fb_num) {

    struct drm_mode_crtc_page_flip page_flip = {0};

    page_flip.crtc_id = control->crtc.crtc_id;
    page_flip.fb_id = control->cmd_dumb[fb_num].fb_id;
    if(ioctl(control->dri_fd, DRM_IOCTL_MODE_PAGE_FLIP, &page_flip) < 0) {
        fprintf(stderr, "%d %s         ", errno, strerror(errno));
        fprintf(stderr, "Failed to flip the page\n");
        return FAILURE;
    }
    control->current_fb = fb_num;

    return SUCCESS;
}


/*
 *  Opens the requested DRI port from /dev/dri/card*
 *  Places the file descriptor at the location of fd
 *
 *  returns 0 on success
 *  returns -1 on failure
 */
static int openPort(char * port, int * fd) {
    int dri_fd = open(port, O_RDWR, O_CLOEXEC);
    if(dri_fd == -1) {
        return FAILURE;
    }

    *fd = dri_fd;
    return SUCCESS;
}


/*
 *  Closes all open file descriptors and frees all allocated memory
 *
 *  returns 0 on success
 *  returns -1 on failure
 */
int drmControlClose(drm_cntrl * control) {
    for(int i = 0; i < NUM_DUMB_BUFFERS; i++) {
        munmap(control->fbMem[i], control->create_dumb[i].size);
    }

    if(ioctl(control->dri_fd, DRM_IOCTL_DROP_MASTER, 0) < 0) {
        fprintf(stderr, "%d %s         ", errno, strerror(errno));
        fprintf(stderr, "Failed to drop the master\n");
        return FAILURE;
    }

    close(control->dri_fd);
    return SUCCESS;
}

/*
 *  Creates a new resolution mode on the connector and CRTC
 *  Creates two new frame buffers to be used at the new resolution mode
 *  Populates the new data into the drm_cntrl structure
 *
 *  returns 0 on success
 *  returns -1 on failure
 */
int setNewResolution(char * port, struct drm_mode_modeinfo * mode, drm_cntrl * control) {
    drmControlClose(control);

    return drmControlInit(port, mode, control);
   }
