/*
 *  datasource.h
 *  showeq
 *
 *  Created by Stephen Raub on 4/26/09.
 *  Copyright 2009 Vexislogic. All rights reserved.
 *
 */

#ifndef __DATASOURCE_H__
#define __DATASOURCE_H__

#include <QObject>

#include "packetcommon.h"
#include "packetinfo.h"
#include "packet.h"

/* [brainiac] this file defines an interface for classes that provide showeq
 * with a source from which data can be processed. I stray from the term "packets"
 * because ideally this data can come from any source, not just a network stream. */

class DataSource : public QObject
{
	Q_OBJECT
	
public:

	DataSource(QObject* parent = NULL);

	virtual ~DataSource();

	/* the core function for this interface. Use it to connect the data source to
	 * classes that will consume the data. */
	virtual bool connectReceiver(const QString& messageName, const QString& payloadName,
		const QObject* receiver, const char* member)
	{
		return connectReceiver(messageName, payloadName, SP_Zone, DIR_Server | DIR_Client,
			SZC_None, receiver, member);
	}
	
	virtual bool connectReceiver(const QString& messageName, const QString& payloadName,
		EQStreamPairs sp, uint8_t dir, EQSizeCheckType szt, const QObject* receiver, const char* member) = 0;
	
	/* update is called every time the game state should be updated */
	virtual void update() = 0;
	


	/* starts operation. Used when the data source is to be activated. How this
	 * is achieved is left up to the implementation. */
	virtual void start() = 0;
	
	/* stops operation. Used when the data source is deactivated. How this is
	 * achieved is left up to the implementation. */
	virtual void stop() = 0;
	
	/* resets the data source and clears the operation. This should clear out any
	 * pre-existing state and make it ready for a new one. */
	virtual void reset();
};

/* TODO: Implement DataSourceFactory class */
class DataSourceFactory : public QObject
{
	Q_OBJECT

public:
	DataSourceFactory(QObject* parent = NULL);

	virtual ~DataSourceFactory();

	virtual DataSource* create();

};


#endif // __DATASOURCE_H__

