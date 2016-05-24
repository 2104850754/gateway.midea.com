# gateway.midea.com
美的网关服务器
测试方法
1.在本地搭建一个本地http服务器，将Firmware.bin，version，zcbSoftware.bin放到服务器中
  由于只是测试，只要保证version是对的即可，bin文件可以随便整个
2.将zcbconfig.cfg放到/ert/share/iot/中，这个是网关的配置文件夹，类似串口，deviceID，SN什么的都放在里面
3.编译运行即可
