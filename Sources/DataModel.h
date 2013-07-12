/* 
 * Copyright (c) 2010 Ferruccio Vitale <unixo@devzero.it>
 * All rights reserved. 
 *  
 * $Id: DataModel.h 75 2010-09-13 09:25:53Z unixo $ 
 */

#ifndef __DATAMODEL_H__
#define __DATAMODEL_H__

#include "common.h"

using namespace std;

/**
   An DataModel object describes a schema — a collection of entities
   (data models) that you use in your application.
   The model contains one or more objects representing the entities 
   in the schema. Each entity name object has property description 
   objects that represent the properties (or fields) of the entity 
   in the schema.
 */
class DataModel : public Singleton<DataModel>
{
private:
    map<std::string, StringSet *> _models;

protected:
    friend class Singleton<DataModel>;
    DataModel();
    virtual ~DataModel();
    
public:
    StringSet & keysForEntity(string anEntity);
};

#endif /* __DATAMODEL_H__ */
