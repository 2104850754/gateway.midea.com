/*
 ============================================================================
 Name        : midea_ota.c
 Author      : Humle
 Version     :
 Copyright   : Your copyright notice
 Description : OTA in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
 *Note updateURL is pushed by APP.
 *The zcb gateway should subscribe the topic /ota/$deviceID
 *The APP should public this topic and transport the payload:
   {
	     "msgid":"xxxxxx",
	     "code":"xxxxxx",
         "data":{
                    "ota.start":"updateURL"
                }
	}
 *But now, just for test, we set a fixed value here
 */
#define updateURL "http://localhost"

#define CMDBUF 1024

int getVersion(char *versionFile ,int *softWareVersion ,int *firmwareVersion){
	  FILE * f;
	  char line[1024];
	  char* k, *v;
	//  f = fopen(cfgfile, "r");
	  f = fopen(versionFile, "r");
	  if (!f)
	    return 0;
	  while (!feof(f))
	  {
	    if (!fgets(line, sizeof(line), f))
	      break;
	    k = strtok(line, "=");
	    v = strtok(0, "=");
	    if (k && v)
	    {
            if(!strcmp(k,"firmwareVersion")){
            	*firmwareVersion = strtol(v,NULL,0);
            }
            if(!strcmp(k,"softwareVersion")){
            	*softWareVersion = strtol(v,NULL,0);
            }
	    }
	  }
	  fclose(f);
	  return 1;
}

int main(void) {

	char cmd[CMDBUF];
	int status = 0;

	int cur_softwareversion=0,cur_firmwareversion=0;
	int new_softwareversion=0,new_firmwareversion=0;
	int shouldUpdateSoftWare=0,shouldUpdateFirmware=0;


//Get the current version
	if(!getVersion("/usr/share/iot/zcbconfig.cfg",&cur_softwareversion,&cur_firmwareversion)){
        return EXIT_FAILURE;
	}

	memset(cmd,0,CMDBUF);
//Download the version file
    snprintf(cmd,CMDBUF,"curl -o /tmp/version %s/%s",updateURL,"version");

    system(cmd);
    wait(&status);
    if(!WIFEXITED(status)){
    	return EXIT_FAILURE;
    }

//Get the new version
	if(!getVersion("/tmp/version",&new_softwareversion,&new_firmwareversion)){
        return EXIT_FAILURE;
	}
//Update the zcb software
	if(new_softwareversion > cur_softwareversion){
		status = 0;
		memset(cmd,0,CMDBUF);
	    snprintf(cmd,CMDBUF,"curl -o /tmp/version %s/%s",updateURL,"zcbSoftware.bin");
	    system(cmd);
	    wait(&status);
	    if(!WIFEXITED(status)){
	    	return EXIT_FAILURE;
	    }
	    shouldUpdateSoftWare = 1;
	}
//update the coordinator firmware
	if(new_firmwareversion > cur_softwareversion){
		status = 0;
		memset(cmd,0,CMDBUF);
	    snprintf(cmd,CMDBUF,"curl -o /tmp/version %s/%s",updateURL,"Firmware.bin");
	    system(cmd);
	    wait(&status);
	    if(!WIFEXITED(status)){
	    	return EXIT_FAILURE;
	    }
	    shouldUpdateFirmware =1;
	}

	if((shouldUpdateSoftWare ==1)||(shouldUpdateFirmware == 1)){
		status = 0;
		system("./update.sh shouldUpdateSoftWare shouldUpdateFirmware");
	    wait(&status);
	    if(!WIFEXITED(status)){
	    	return EXIT_FAILURE;
	    }
	}
//Compare current version and the new version and decide if should update
	return EXIT_SUCCESS;
}
