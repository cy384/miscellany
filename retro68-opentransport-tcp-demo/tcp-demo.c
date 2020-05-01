/*
 * A simple TCP demo that sends an HTTP request
 * Adapted for use with the Retro68 toolchain by cy384 in 2020
 *
 * based on Apple sample code "OTSimpleDownloadHTTP.c" which has the header:

Written by: Quinn "The Eskimo!"

Copyright:	Copyright (C) 1997-1999 by Apple Computer, Inc., All Rights Reserved.

You may incorporate this Apple sample source code into your program(s) without
restriction. This Apple sample source code has been provided "AS IS" and the
responsibility for its operation is yours. You are not permitted to redistribute
this Apple sample source code as "Apple sample source code" after having made
changes. If you're going to re-distribute the source, we require that you make
it clear in the source that the code was descended from Apple sample source
code, but that you've made changes.
 */

#include <stdio.h>

#include <OpenTransport.h>
#include <OpenTptInternet.h>
#include <Threads.h>

void assertp(char* message, int b)
{
	if (!b) printf("assert fail: %s\n", message);
}

enum { buffer_size = 4096 };

// event handler to yield whenever we're blocked
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

OSStatus http_request_and_print(const char *hostName, const char *httpCommand)
{
	OSStatus err = noErr;
	Ptr buffer = nil;
	EndpointRef endpoint = kOTInvalidEndpointRef;
	TCall sndCall;
	DNSAddress hostDNSAddress;
	OTFlags junkFlags;
	OTResult bytesSent;
	OTResult bytesReceived;
	OTResult lookResult;
	Boolean bound = false;

	// allocate buffer
	buffer = OTAllocMem(buffer_size);
	if (buffer == nil)
	{
		err = kENOMEMErr;
	}

	// open TCP endpoint
	if (err == noErr)
	{
		endpoint = OTOpenEndpoint(OTCreateConfiguration(kTCPName), 0, nil, &err);
	}

	// configure the endpoint
	// synchronous and blocking, and we yield until we get a result
	if (err == noErr)
	{
		OSStatus result;

		result = OTSetSynchronous(endpoint);
		assertp("OTSetSynchronous failed", result == noErr);

		result = OTSetBlocking(endpoint);
		assertp("OTSetBlocking failed", result == noErr);

		result = OTInstallNotifier(endpoint, yield_notifier, nil);
		assertp("OTInstallNotifier failed", result == noErr);

		result = OTUseSyncIdleEvents(endpoint, true);
		assertp("OTUseSyncIdleEvents failed", result == noErr);

		/* Bind the endpoint. */
		err = OTBind(endpoint, nil, nil);
		assertp("OTBind failed", err == noErr);
		bound = (err == noErr);
	}

	// set up address struct and connect
	if (err == noErr)
	{
		OTMemzero(&sndCall, sizeof(TCall));

		sndCall.addr.buf = (UInt8 *) &hostDNSAddress;
		sndCall.addr.len = OTInitDNSAddress(&hostDNSAddress, (char *) hostName);

		err = OTConnect(endpoint, &sndCall, nil);
		assertp("OTConnect failed", err == noErr);
	}

	// send the contents of the buffer
	if (err == noErr)
	{
		int sent = OTSnd(endpoint, (void *) httpCommand, OTStrLength(httpCommand), 0);

		printf("bytes in buffer: %d", OTStrLength(httpCommand));
		printf("bytes sent: %d\n", sent);

		if (sent > 0)
		{
			err = noErr;
		}
		else
		{
			assertp("OTSnd failed", sent == 0);
			err = sent;
		}
	}

	// if we sent bytes, receive from the server
	if (err == noErr)
	{
		do
		{
			int received = OTRcv(endpoint, (void *) buffer, buffer_size, &junkFlags);

			// if we got bytes, print them
			if (received > 0)
			{
				for (int i = 0; i < received; i++) printf("%c", buffer[i]);
			}
			else
			{
				err = received;
			}

		} while (err == noErr); // stop once we get an error (i.e. disconnected)
	}

	// handle "errors"
	if (err == kOTLookErr)
	{
		printf("\n");

		lookResult = OTLook(endpoint);

		switch (lookResult)
		{
			case T_DISCONNECT:
				printf("disconnected\n");
				err = OTRcvDisconnect(endpoint, nil);
				break;

			case T_ORDREL:
				err = OTRcvOrderlyDisconnect(endpoint);
				printf("orderly disconnected\n");
				if (err == noErr)
				{
					err = OTSndOrderlyDisconnect(endpoint);
				}
				break;

			default:
				printf("other connection error type: %d\n", lookResult);
				break;
		}
	}

	// if it worked and we're bound, unbind
	if ((err == noErr) && bound )
	{
		OSStatus result = OTUnbind(endpoint);
		assertp("OTUnbind failed", result == noErr);
	}

	// if we set up an endpoint, close it
	if (endpoint != kOTInvalidEndpointRef)
	{
		OSStatus result = OTCloseProvider(endpoint);
		assertp("OTCloseProvider failed", result == noErr);
	}

	// if we got a buffer, release it
	if (buffer != nil) OTFreeMem(buffer);

	return err;
}

int main(int argc, char** argv)
{
	const char req[] = "GET / HTTP/1.1\r\nHost: www.cy384.com\r\n\r\n";
	const char hostname_and_port[] = "www.cy384.com:80";

	if (InitOpenTransport() != noErr)
	{
		printf("failed to init\n");
		return 0;
	}

	OSStatus err = http_request_and_print(hostname_and_port, req);
	printf("result: %d\n", err);

	CloseOpenTransport();

	printf("\n(return to exit)\n");
	getchar();

	return 0;
}
