//---------------------------------------------------------------------------
#ifndef userH
#define userH

//---------------------------------------------------------------------------

struct TUser : public TRecord {
	String login;
	String password;
	int profile_id;
	int group_id;
	String description;
	String settings;
	int is_admin;

	TUser();

	String GetName() const;
};

#endif
