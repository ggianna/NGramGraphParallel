#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <fstream>

typedef class MyRecord
{
  public:
    int x, y;
    float z;

    MyRecord(){};
    MyRecord(int x,int y){
      this->x = x;
      this->y = y;
    }
    ~MyRecord(){};
  template <class Archive>
  void serialize( Archive & ar )
  {
    ar( x, y, z );
  }
}MyRecord;
    
typedef class SomeData
{
  
  
  public:
    int32_t id;
    std::shared_ptr<MyRecord> data;
    SomeData(){};
    ~SomeData(){};
    template <class Archive>
    void save( Archive & ar ) const
    {
      ar( data );
    }
        
    template <class Archive>
    void load( Archive & ar )
    {
      static int32_t idGen = 0;
      id = idGen++;
      ar( data );
    }
}SomeData;

int main()
{
  std::ofstream os("out.cereal", std::ios::binary);
  cereal::BinaryOutputArchive oarchive( os );

  SomeData myData;
  myData.data = std::make_shared<MyRecord>(MyRecord(5,3));
  std::cout<<"saved Data Coords: ("<< myData.data->x << ","<<myData.data->y<<")"<<std::endl;
  oarchive( myData );
  os.close();
  std::ifstream is("out.cereal", std::ios::binary);
  cereal::BinaryInputArchive iarchive(is);
  SomeData loadedData;
  iarchive(loadedData);
  std::cout<<"loaded Data Coords: ("<<loadedData.data->x << ","<<loadedData.data->y<<")"<<std::endl;
  is.close();
  return 0;
}