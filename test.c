/* EE 468:  Test for device driver project 
 *
 * You will likely have to change the open() since your
 * device name may be different.
 *
 * The test will write and read character strings into
 * the device.  Then it displays, using printf, the
 * results of the reads.  Record these outputs from
 * the printfs into a file.
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

int main()
{

	int fp;
	char buffer_in[100];
	char buffer_out[100];
	int i;
	char c;

	fp = open("/dev/ee468Driver", O_RDWR); /* Change "myDev" to your own device. */

	read(fp,buffer_out,10); /* Clear out your device */

	strcpy(buffer_in,"abcdefghijklmnopqrstuvwxyz"); /* Initialize buffer_in */

	write(fp, buffer_in, 10); /* This fills the device buffer */
	strcpy(buffer_out,"                           ");  /* Initializes buffer_out*/
										 /* with spaces */
	read(fp,buffer_out,10);
	printf("Write and read 10 characters:  %s\n\n",buffer_out);

	write(fp, buffer_in, 5); /* Fill half the device */
	strcpy(buffer_out,"                           ");
	read(fp,buffer_out,2);   /* Read two characters */
	printf("Write 5 chars and read 2:  %s\n\n",buffer_out);

	write(fp, buffer_in, 2); /* Fill in two more characters */
	strcpy(buffer_out,"                           ");
	read(fp,buffer_out,5);   /* Read all characters in device  */
	printf("Write 2 and read 5 chars:  %s\n\n",buffer_out);

	write(fp, buffer_in, 20);
	strcpy(buffer_out,"                           ");
	read(fp,buffer_out,20);
	printf("Write 20 chars (overflow to at most 10) and read 20 (should read only 10, underflow):  %s\n\n",buffer_out);

	printf("End of test\n");
}

