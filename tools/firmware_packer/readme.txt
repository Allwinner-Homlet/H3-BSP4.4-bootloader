使用说明：

在顶层目录(firmware_packer/)下：
Usage: fwpacker -in src_file_path -out dest_file_path
Option:
     -exec               export | pack
     -system             (depend on -exec)system_file_path
     -boot0_sd           (depend on -exec)boot0_sdcard_file_path
     -boot0_nd           (depend on -exec)boot0_nand_file_path
     -uboot              (depend on -exec)uboot_file_path
     -recovery           (depend on -exec)recovery_file_path
     -toc0               (depend on -exec)toc0_file_path
     -toc1               (depend on -exec)toc1_file_path
     -boot               (depend on -exec)boot_file_path

     例如：
	 替换文件：
         用system.img替换fw.img固件中的system镜像，生成新的fw_new.img:
			fwpacker -in fw.img -exec pack -system system.img -out fw_new.img
		可以同时替换多个文件：
			fwpacker -in fw.img -exec pack -system system.img -uboot uboot.fex -out fw_new.img

	 提取文件：
         提取出fw.img中的system镜像，生成system_out.img:
			fwpacker -in fw.img -exec export -system system_out.img
 		可以同时提取多个文件：
			fwpacker -in fw.img -exec export -system system_out.img -uboot uboot_out.img

 工具源码目录结构如下：
 .
 ├── config/               -->配置文件
 ├── FWmdf/                -->编译FWmdf.so的源码
 ├── FWmdf.so              -->fwpacker工具所需要的库
 ├── fwpacker              -->工具
 ├── include/              -->头文件
 ├── main/                 -->编译fwpacker的源码
 ├── Makefile              -->编译FWmdf/和main/这两个目录下的源码
 └── readme.txt            -->使用说明


 编译说明：
     在顶层目录(firmware_packer/)目录下，执行make，会一起棉衣FWmdf和main的源码，并将编译目标(fwpackere、FWmdf.so)输出到顶层目录下；
     编译完成后在顶层目录下使用fwpcker执行固件相关操作。
