/// MorseGraph.hpp
/// Shaun Harker
/// 2015-05-24

#ifndef DSGRN_MORSEGRAPH_HPP
#define DSGRN_MORSEGRAPH_HPP

#ifndef INLINE_IF_HEADER_ONLY
#define INLINE_IF_HEADER_ONLY
#endif

#include "MorseGraph.h"

INLINE_IF_HEADER_ONLY MorseGraph::
MorseGraph ( void ) { 
  data_ . reset ( new MorseGraph_ );
}

INLINE_IF_HEADER_ONLY Poset const MorseGraph::
poset ( void ) const {
  return data_ ->poset_;
}

INLINE_IF_HEADER_ONLY Annotation const MorseGraph::
annotation ( uint64_t v ) const {
  return data_ ->annotations_ . find ( v ) -> second;
}

INLINE_IF_HEADER_ONLY std::string MorseGraph::
stringify ( void ) const {
  std::stringstream ss;
  ss << "{\"poset\":";
  ss << data_ -> poset_ . stringify ();
  ss << ",\"annotations\":[";
  uint64_t N = data_ -> poset_ . size ();
  bool first = true;
  for ( uint64_t v = 0; v < N; ++ v ) {
    if ( first ) first = false; else ss << ",";
    ss << annotation ( v ) . stringify ();
  }
  ss << "]}";
  return ss . str ();
}

INLINE_IF_HEADER_ONLY void MorseGraph::
parse ( std::string const& str ) {
  json mg = json::parse(str);
  data_ -> poset_ . parse ( json::stringify ( mg["poset"] )); //TODO: inefficient
  data_ -> annotations_ . clear ();
  json annotation_array = mg["annotations"];
  uint64_t N = annotation_array . size ();
  for ( uint64_t v = 0; v < N; ++ v ) {
    data_ -> annotations_ [ v ] . parse ( json::stringify ( annotation_array[v] )); //TODO: inefficient
  } 
}


INLINE_IF_HEADER_ONLY std::ostream& operator << ( std::ostream& stream, MorseGraph const& md ) {
  Poset const poset = md . poset ();
  stream << "digraph {\n";
  for ( uint64_t v = 0; v < poset . size (); ++ v ) {
    stream << v;
    stream << "[label=\"";
    bool first_item = true;
    for ( auto s : md . annotation ( v ) ) {
      if ( first_item ) first_item = false; else stream << ", ";
      stream << s;
    }
    stream << "\"];\n";
  }
  for ( uint64_t source = 0; source < poset . size (); ++ source ) {
    for ( uint64_t target : poset . adjacencies ( source ) ) {
      stream << source << " -> " << target << ";\n";
    }
  }
  stream << "}\n";
  return stream;
}

INLINE_IF_HEADER_ONLY std::string MorseGraph::
SHA256 ( void ) const {
  std::stringstream ss;
  ss << *this;
  return sha256 ( ss . str () );
}
  
INLINE_IF_HEADER_ONLY void MorseGraph::
_canonicalize ( void ) {
}

#endif
