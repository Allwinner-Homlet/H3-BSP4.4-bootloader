ʹ��˵����

�ڶ���Ŀ¼(firmware_packer/)�£�
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

     ���磺
	 �滻�ļ���
         ��system.img�滻fw.img�̼��е�system���������µ�fw_new.img:
			fwpacker -in fw.img -exec pack -system system.img -out fw_new.img
		����ͬʱ�滻����ļ���
			fwpacker -in fw.img -exec pack -system system.img -uboot uboot.fex -out fw_new.img

	 ��ȡ�ļ���
         ��ȡ��fw.img�е�system��������system_out.img:
			fwpacker -in fw.img -exec export -system system_out.img
 		����ͬʱ��ȡ����ļ���
			fwpacker -in fw.img -exec export -system system_out.img -uboot uboot_out.img

 ����Դ��Ŀ¼�ṹ���£�
 .
 ������ config/               -->�����ļ�
 ������ FWmdf/                -->����FWmdf.so��Դ��
 ������ FWmdf.so              -->fwpacker��������Ҫ�Ŀ�
 ������ fwpacker              -->����
 ������ include/              -->ͷ�ļ�
 ������ main/                 -->����fwpacker��Դ��
 ������ Makefile              -->����FWmdf/��main/������Ŀ¼�µ�Դ��
 ������ readme.txt            -->ʹ��˵��


 ����˵����
     �ڶ���Ŀ¼(firmware_packer/)Ŀ¼�£�ִ��make����һ������FWmdf��main��Դ�룬��������Ŀ��(fwpackere��FWmdf.so)���������Ŀ¼�£�
     ������ɺ��ڶ���Ŀ¼��ʹ��fwpckerִ�й̼���ز�����
