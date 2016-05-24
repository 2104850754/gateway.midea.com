#!/bin/sh
shouldUpdateSoftWare=$1
shouldUpdateFirmware=$2
/etc/init.d/iot_zb_initd stop
if [ $shouldUpdateSoftWare -eq 1 ]; then
echo "Now we will update the zcb software"
cp /tmp/zcbSoftware.bin /usr/bin/
fi

if [ $shouldUpdateFirmware -eq 1 ]; then
echo "Now we will update the coordinator firmware"
/usr/bin/JennicModuleAutoProgram.sh
fi

/etc/init.d/iot_zb_initd start
