/* Auto-generated by genmsg_cpp for file /home/fer/fuerte_workspace/sandbox/find_object_2d/msg/PointObjects.msg */
#ifndef FIND_OBJECT_2D_MESSAGE_POINTOBJECTS_H
#define FIND_OBJECT_2D_MESSAGE_POINTOBJECTS_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"

#include "find_object_2d/Point_id.h"

namespace find_object_2d
{
template <class ContainerAllocator>
struct PointObjects_ {
  typedef PointObjects_<ContainerAllocator> Type;

  PointObjects_()
  : objeto()
  {
  }

  PointObjects_(const ContainerAllocator& _alloc)
  : objeto(_alloc)
  {
  }

  typedef std::vector< ::find_object_2d::Point_id_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::find_object_2d::Point_id_<ContainerAllocator> >::other >  _objeto_type;
  std::vector< ::find_object_2d::Point_id_<ContainerAllocator> , typename ContainerAllocator::template rebind< ::find_object_2d::Point_id_<ContainerAllocator> >::other >  objeto;


  typedef boost::shared_ptr< ::find_object_2d::PointObjects_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::find_object_2d::PointObjects_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct PointObjects
typedef  ::find_object_2d::PointObjects_<std::allocator<void> > PointObjects;

typedef boost::shared_ptr< ::find_object_2d::PointObjects> PointObjectsPtr;
typedef boost::shared_ptr< ::find_object_2d::PointObjects const> PointObjectsConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::find_object_2d::PointObjects_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::find_object_2d::PointObjects_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace find_object_2d

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::find_object_2d::PointObjects_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::find_object_2d::PointObjects_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::find_object_2d::PointObjects_<ContainerAllocator> > {
  static const char* value() 
  {
    return "f79b4adb458edaaf81faec054b53de74";
  }

  static const char* value(const  ::find_object_2d::PointObjects_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xf79b4adb458edaafULL;
  static const uint64_t static_value2 = 0x81faec054b53de74ULL;
};

template<class ContainerAllocator>
struct DataType< ::find_object_2d::PointObjects_<ContainerAllocator> > {
  static const char* value() 
  {
    return "find_object_2d/PointObjects";
  }

  static const char* value(const  ::find_object_2d::PointObjects_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::find_object_2d::PointObjects_<ContainerAllocator> > {
  static const char* value() 
  {
    return "Point_id[] objeto\n\
\n\
================================================================================\n\
MSG: find_object_2d/Point_id\n\
int16 id\n\
geometry_msgs/Point punto\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Point\n\
# This contains the position of a point in free space\n\
float64 x\n\
float64 y\n\
float64 z\n\
\n\
";
  }

  static const char* value(const  ::find_object_2d::PointObjects_<ContainerAllocator> &) { return value(); } 
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::find_object_2d::PointObjects_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.objeto);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct PointObjects_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::find_object_2d::PointObjects_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::find_object_2d::PointObjects_<ContainerAllocator> & v) 
  {
    s << indent << "objeto[]" << std::endl;
    for (size_t i = 0; i < v.objeto.size(); ++i)
    {
      s << indent << "  objeto[" << i << "]: ";
      s << std::endl;
      s << indent;
      Printer< ::find_object_2d::Point_id_<ContainerAllocator> >::stream(s, indent + "    ", v.objeto[i]);
    }
  }
};


} // namespace message_operations
} // namespace ros

#endif // FIND_OBJECT_2D_MESSAGE_POINTOBJECTS_H

