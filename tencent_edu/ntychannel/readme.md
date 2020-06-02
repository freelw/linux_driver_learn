查看我的腾讯课堂

如何测试
1. 在ntychannel文件夹下 make
2. 创建设备文件
    sudo mknod /dev/ntychannel c 96 0
3. sudo insmod ntychannel.ko
4. dmesg 查看是否安装成功
5. 在ntyusermodetest文件夹下make
6. 调用channeltest
7. echo "hello channel" > /dev/ntychannel