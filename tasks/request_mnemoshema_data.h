//---------------------------------------------------------------------------
#ifndef request_mnemoshema_dataH
#define request_mnemoshema_dataH
//---------------------------------------------------------------------------
//enum TTaskType {HTTP = 1, SMB = 2, DEVICE_SMB = 3, ALARM_SMB = 4};
//---------------------------------------------------------------------------
class TTaskRequestMnemoshemaData :
	public sysThread::TThreadTask,
	public sysPatterns::IPooledObject,
	public sysObserverable::TObserverable {
	public:
		//the function for clean up resources after the task was finished
		virtual void CleanUp() = 0;
};
#endif
