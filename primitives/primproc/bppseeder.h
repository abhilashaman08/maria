/* Copyright (C) 2014 InfiniDB, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA. */

//
// $Id: bppseeder.h 2035 2013-01-21 14:12:19Z rdempsey $
// C++ Interface: bppseeder
//
// Description:
//
//
// Author: Patrick <pleblanc@localhost.localdomain>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <tr1/unordered_map>
#include <boost/shared_ptr.hpp>

#include "batchprimitiveprocessor.h"
#include "primitiveserver.h"
#include "umsocketselector.h"

namespace primitiveprocessor
{
class BPPSeeder : public threadpool::FairThreadPool::Functor
{
 public:
  BPPSeeder() = delete;
  BPPSeeder(const messageqcpp::SBS&, const SP_UM_MUTEX& wLock, const SP_UM_IOSOCK& ios, const int pmThreads,
            const bool trace = false);
  BPPSeeder(const BPPSeeder& b);

  ~BPPSeeder() override;

  int operator()() override;

  bool isSysCat();
  boost::shared_ptr<std::ofstream> spof;

  uint32_t getID();

  void priority(uint32_t p)
  {
    _priority = p;
  }
  uint32_t priority()
  {
    return _priority;
  }
  size_t getWeight() const
  {
    assert(bpp);
    return bpp->getWeight();
  }

 private:
  void catchHandler(const std::string& s, uint32_t uniqueID, uint32_t step);
  void flushSyscatOIDs();

  messageqcpp::SBS bs;
  SP_UM_MUTEX writelock;
  SP_UM_IOSOCK sock;
  int fPMThreads;
  bool fTrace;

  /* To support reentrancy */
  uint32_t uniqueID, sessionID, stepID, failCount;
  boost::shared_ptr<BatchPrimitiveProcessor> bpp;
  SBPPV bppv;
  bool firstRun;
  boost::posix_time::ptime dieTime;

  uint32_t _priority;
};

};  // namespace primitiveprocessor
