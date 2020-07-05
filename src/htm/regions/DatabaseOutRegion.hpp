/* ---------------------------------------------------------------------
 * HTM Community Edition of NuPIC
 * Copyright (C) 2013, Numenta, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero Public License for more details.
 *
 * You should have received a copy of the GNU Affero Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 * --------------------------------------------------------------------- */

/** @file
 * Declarations for DatabaseOutRegion class
 */

//----------------------------------------------------------------------

#ifndef SRC_HTM_REGIONS_DATABASEOUTREGION_HPP_
#define SRC_HTM_REGIONS_DATABASEOUTREGION_HPP_

//----------------------------------------------------------------------

#include <htm/engine/RegionImpl.hpp>
#include <htm/ntypes/Array.hpp>
#include <htm/types/Types.hpp>
#include <htm/types/Serializable.hpp>
#include <htm/ntypes/Value.hpp>

extern "C" {
#include <sqlite3.h>
}

namespace htm {


/**
 *  VectorFileEffector is a node that takes its input vectors and
 *  writes them sequentially to a file.
 *
 *  The current input vector is written (but not flushed) to the file
 *  each time the effector's compute() method is called.
 *
 *  The file format for the file is a space-separated list of numbers, with
 *  one vector per line:
 *
 *        e11 e12 e13 ... e1N
 *        e21 e22 e23 ... e2N
 *           :
 *        eM1 eM2 eM3 ... eMN
 *
 *  VectorFileEffector implements the execute() commands as defined in the
 *  nodeSpec.
 *
 */
class DatabaseOutRegion : public RegionImpl, Serializable {
public:
  static Spec *createSpec();
  size_t getNodeOutputElementCount(const std::string &outputName) const override;
  void setParameterString(const std::string &name, Int64 index,
                          const std::string &s) override;
  std::string getParameterString(const std::string &name, Int64 index) override;

  void initialize() override;

  DatabaseOutRegion(const ValueMap &params, Region *region);

  DatabaseOutRegion(ArWrapper& wrapper, Region *region);

  virtual ~DatabaseOutRegion();


	CerealAdapter;  // see Serializable.hpp
  // FOR Cereal Serialization
  template<class Archive>
  void save_ar(Archive& ar) const {
    ar(cereal::make_nvp("outputFile", filename_));
    ar(CEREAL_NVP(dim_));  // in base class
  }

  // FOR Cereal Deserialization
  template<class Archive>
  void load_ar(Archive& ar) {
    ar(cereal::make_nvp("outputFile", filename_));
		if (filename_ != "")
		      openFile(filename_);
    ar(CEREAL_NVP(dim_));  // in base class
  }

  bool operator==(const RegionImpl &other) const override;
  inline bool operator!=(const DatabaseOutRegion &other) const {
    return !operator==(other);
  }


  void compute() override;

  virtual std::string executeCommand(const std::vector<std::string> &args,
                                     Int64 index) override;

private:
  void closeFile();
  void openFile(const std::string &filename);

    Array dataIn_;
    std::string filename_;          // Name of the output file

    sqlite3 *dbHandle;		//Sqlite3 connection handle

  /// Disable unsupported default constructors
    DatabaseOutRegion(const DatabaseOutRegion &);
    DatabaseOutRegion &operator=(const DatabaseOutRegion &);

}; // end class VectorFileEffector

//----------------------------------------------------------------------

} // namespace htm


#endif /* SRC_HTM_REGIONS_DATABASEOUTREGION_HPP_ */
