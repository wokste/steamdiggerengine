#include "entityhandle.h"
#include "entitylist.h"

EntityHandle::EntityHandle(){
	nr = 0;
	index = 0;
}

EntityHandle::EntityHandle(unsigned int newNr, unsigned int newIndex){
	nr = newNr;
	index = newIndex;
}

Entity* EntityHandle::get(EntityList& list){
	ListItem &info = list.entities[index];
	if ( nr == info.nr ){
		return info.entity;
	}
	else{
		return nullptr;
	}
}
