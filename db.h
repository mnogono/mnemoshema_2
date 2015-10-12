//---------------------------------------------------------------------------

#ifndef dbH
#define dbH
//---------------------------------------------------------------------------

TUser & LoadUser(const String &login, const String &password);

std::list<TCollectionPoint *> & LoadCollectionPoints(const TUser &user);

std::list<TDevice *> & LoadDevices(const TUser &user);

//void UpdateDeviceDataBlockSize(std::list<TDevice *> &devices);

std::list<TSensor *> & LoadSensors(const TUser &user);

std::list<TSensorBit *> & LoadSensorsBits(const TUser & user);

std::list<TDeviceTreeGroup *> & LoadDeviceTreeGroups(const TUser &User);

std::list<TMnemoshema *> & LoadMnemoshems(const TUser &user);

#endif
