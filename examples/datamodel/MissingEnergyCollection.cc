#include "MissingEnergyCollection.h"

MissingEnergyCollection::MissingEnergyCollection() : m_collectionID(0), m_data(new MissingEnergyVector() ){
}

const MissingEnergyHandle& MissingEnergyCollection::get(int index) const{
  return m_handles[index];
}

MissingEnergyHandle& MissingEnergyCollection::create(){
    m_data->emplace_back(MissingEnergy());
    int index = m_data->size()-1;
    m_handles.emplace_back(MissingEnergyHandle(index,m_collectionID, m_data));
    return m_handles.back();
}

void MissingEnergyCollection::clear(){
  m_data->clear();
  m_handles.clear();

}

void MissingEnergyCollection::prepareForWrite(const albers::Registry* registry){

}

void MissingEnergyCollection::prepareAfterRead(albers::Registry* registry){
  m_handles.clear();
  int index = 0;
  for (auto& data : *m_data){
    
    m_handles.emplace_back(MissingEnergyHandle(index,m_collectionID, m_data));
    ++index;
  }
}


void MissingEnergyCollection::setPODsAddress(const void* address){
  m_data = (MissingEnergyVector*)address;
}


const MissingEnergyHandle MissingEnergyCollectionIterator::operator* () const {
  return m_collection->get(m_index);
}

void MissingEnergyCollection::print() const {
  std::cout<<"collection "<<m_collectionID
           <<", buf "<<m_data
           <<", nhandles "<<m_handles.size()<<std::endl;
}