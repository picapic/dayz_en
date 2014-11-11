#if defined _samp_addon
	#endinput
#endif
#define _samp_addon


native InitAddon(ip[], port, maxplayers);
native IsClientConnected(clientid);
native KickClient(clientid);
native GetClientSerial(clientid);
native GetClientScreenshot(clientid, remote_filename[]);
native TransferLocalFile(file[], toclient, remote_filename[]);
native TransferRemoteFile(remote_filename[], fromclient, file[]);
