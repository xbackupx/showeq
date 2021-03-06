	/*
 * filternotifications.cpp
 *
 *  ShowEQ Distributed under GPL
 *  http://seq.sourceforge.net/
 *
 * Portions Copyright 2003 Zaphod (dohpaz@users.sourceforge.net).
 *
 */

#ifndef _FILTERNOTIFICATIONS_H_
#define _FILTERNOTIFICATIONS_H_

class Item;

class FilterNotifications : public QObject
{
	Q_OBJECT

public:
	FilterNotifications(QObject* parent = 0, const char* name = 0);
	~FilterNotifications();

	bool useSystemBeep() const { return m_useSystemBeep; }
	bool useCommands() const { return m_useCommands; }

public slots:
	void setUseSystemBeep(bool val);
	void setUseCommands(bool val);

	void addItem(const Item* item);
	void delItem(const Item* item);
	void killSpawn(const Item* item);
	void changeItem(const Item* item, uint32_t changeType);

protected:
	void handleAlert(const Item* item, const QString& commandPref, const QString& cue);
	void executeCommand(const Item* item, const QString& rawCommand, const QString& audioCue);
	void beep();

	bool m_useSystemBeep;
	bool m_useCommands;
};

#endif // _FILTERNOTIFICATIONS_H_
