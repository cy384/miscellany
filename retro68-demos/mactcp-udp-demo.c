#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Devices.h>
#include <MacTCP.h>

void assertp(char* message, int b)
{
	if (!b) printf("%s\n", message);
}

ip_addr ip_str_to_int(const char* ip_str)
{
	int a, b, c, d;
	sscanf(ip_str, "%d.%d.%d.%d", &a, &b, &c, &d);
	return (a << 24) + (b << 16) + (c << 8) + d;
}

int main(int argc, char** argv)
{
	const char udp_message[] = "1234567890";
	const char* ip_str = "162.243.192.150";
	udp_port port = 12345;
	int buffer_size = 2048;

	int message_length = strlen(udp_message);
	ip_addr remote_ip_addr = ip_str_to_int(ip_str);

	char input_buffer[buffer_size];

	printf("sending \"%s\" to %s:%d\n", udp_message, ip_str, port);

	OSErr err = 0;

	struct wdsEntry wds[2] = { {(unsigned short)message_length, (Ptr)udp_message}, {0, NULL} };

	short MTCPDriver;
	err = OpenDriver("\p.IPP", &MTCPDriver);
	assertp("OpenDriver failed", noErr == err);

	StreamPtr udp_stream;

	// set up stream
	UDPiopb open_pb;
	memset(&open_pb, 0, sizeof(open_pb));

	open_pb.csCode = UDPCreate;
	open_pb.ioCRefNum = MTCPDriver;

	open_pb.csParam.create.rcvBuff = input_buffer;
	open_pb.csParam.create.rcvBuffLen = buffer_size;
	open_pb.csParam.create.notifyProc = NULL; // null: no ASR
	open_pb.csParam.create.localPort = 0; // 0: some unused port
	open_pb.csParam.create.userDataPtr = NULL; // given as param to the ASR
	open_pb.csParam.create.endingPort = 0; // used for multiport creates

	err = PBControlSync((ParmBlkPtr)&open_pb);
	assertp("open PBControlSync failed", noErr == err);

	udp_stream = open_pb.udpStream;

	// send
	UDPiopb send_pb;
	memset(&send_pb, 0, sizeof(send_pb));

	send_pb.ioCRefNum = MTCPDriver;
	send_pb.udpStream = udp_stream;
	send_pb.csCode = UDPWrite;
	send_pb.csParam.send.remoteHost = remote_ip_addr;
	send_pb.csParam.send.remotePort = port;
	send_pb.csParam.send.wdsPtr = (Ptr)wds;
	send_pb.csParam.send.checkSum = 0; // 0: don't compute checksum
	send_pb.csParam.send.sendLength = message_length;
	send_pb.csParam.send.userDataPtr = NULL;
	send_pb.csParam.send.localPort = 0; // 0: some unused port

	err = PBControlSync((ParmBlkPtr)&send_pb);
	assertp("send PBControlSync failed", noErr == err);

	// check before releasing
	while (open_pb.ioResult > 0 || send_pb.ioResult > 0);

	// release
	UDPiopb close_pb;
	memset(&close_pb, 0, sizeof(close_pb));

	close_pb.ioCRefNum = MTCPDriver;
	close_pb.udpStream = udp_stream;
	close_pb.csCode = UDPRelease;
	err = PBControlSync((ParmBlkPtr)&close_pb);
	assertp("close PBControlSync failed", noErr == err);

	printf("\ndone.\n(return to exit)\n");
	getchar();

	return 0;
}
