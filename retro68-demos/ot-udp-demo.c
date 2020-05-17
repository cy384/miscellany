#include <stdio.h>

#include <OpenTransport.h>
#include <OpenTptInternet.h>
#include <Threads.h>

void assertp(char* message, int b)
{
	if (!b) printf("assert fail: %s\n", message);
}

static pascal void yield_notifier(void* contextPtr, OTEventCode code, OTResult result, void* cookie)
{
	OSStatus err;

	switch (code)
	{
		case kOTSyncIdleEvent:
			err = YieldToAnyThread();
			assertp("YieldToAnyThread failed", err == noErr);
			break;

		default:
			break;
	}
}

int main(int argc, char** argv)
{
	const char udp_message[] = "1234567890";
	const char hostname_and_port[] = "www.cy384.com:12345";

	EndpointRef endpoint = kOTInvalidEndpointRef;
	DNSAddress hostDNSAddress;
	TUnitData udp_packet;
	OSStatus err;

	printf("sending \"%s\" to \"%s\"\n", udp_message, hostname_and_port);

	err = InitOpenTransport();
	assertp("InitOpenTransport failed", err == noErr);

	endpoint = OTOpenEndpoint(OTCreateConfiguration(kUDPName), 0, nil, &err);
	assertp("OTOpenEndpoint failed", err == noErr);

	err = OTSetSynchronous(endpoint);
	assertp("OTSetSynchronous failed", err == noErr);

	err = OTSetBlocking(endpoint);
	assertp("OTSetBlocking failed", err == noErr);

	err = OTInstallNotifier(endpoint, yield_notifier, nil);
	assertp("OTInstallNotifier failed", err == noErr);

	err = OTUseSyncIdleEvents(endpoint, true);
	assertp("OTUseSyncIdleEvents failed", err == noErr);

	err = OTBind(endpoint, nil, nil);
	assertp("OTBind failed", err == noErr);

	OTMemzero(&udp_packet, sizeof(TUnitData));

	udp_packet.addr.buf = (UInt8 *) &hostDNSAddress;
	udp_packet.addr.len = OTInitDNSAddress(&hostDNSAddress, (char*) hostname_and_port);
	udp_packet.udata.buf = (void *) udp_message;
	udp_packet.udata.len = OTStrLength(udp_message);

	err = OTSndUData(endpoint, &udp_packet);
	assertp("OTSndUData failed", err == noErr);

	err = OTUnbind(endpoint);
	assertp("OTUnbind failed", err == noErr);

	err = OTCloseProvider(endpoint);
	assertp("OTCloseProvider failed", err == noErr);

	CloseOpenTransport();

	printf("\ndone.\n(return to exit)\n");
	getchar();

	return 0;
}
