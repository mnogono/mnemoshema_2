//---------------------------------------------------------------------------

#ifndef MNEMOSHEMA_RECORD_H
#define MNEMOSHEMA_RECORD_H
//---------------------------------------------------------------------------

struct TMnemoshema : public TRecord {
	String name;
	String structure;

	TMnemoshema();

	String GetName() const;
};

#endif
