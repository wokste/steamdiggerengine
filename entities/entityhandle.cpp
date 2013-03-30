#include "entityhandle.h"
#include "entitylist.h"

EntityHandle::EntityHandle(){
	m_nr = 0;
	m_index = 0;
}

EntityHandle::EntityHandle(unsigned int nr, unsigned int index){
	m_nr = nr;
	m_index = index;
}

Entity* EntityHandle::get(EntityList& list){
	ListItem &info = list.m_entities[m_index];
	if ( m_nr == info.m_nr ){
		return info.m_pEntry;
	}
	else{
		return nullptr;
	}
}
