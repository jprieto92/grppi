/**
* @version		GrPPI v0.2
* @copyright		Copyright (C) 2017 Universidad Carlos III de Madrid. All rights reserved.
* @license		GNU/GPL, see LICENSE.txt
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You have received a copy of the GNU General Public License in LICENSE.txt
* also available in <http://www.gnu.org/licenses/gpl.html>.
*
* See COPYRIGHT.txt for copyright notices and details.
*/
#include <atomic>
#include <experimental/optional>

#include <gtest/gtest.h>

#include "stream_reduce.h"
#include "common/polymorphic_execution.h"

#include "supported_executions.h"

using namespace std;
using namespace grppi;

template <typename T>
class stream_reduce_test : public ::testing::Test {
public:
  T execution_;
  polymorphic_execution poly_execution_ = 
    make_polymorphic_execution<T>();

  // Variables
  int out;
  int window;
  int offset;

  // Vectors
  vector<int> v{};
  
  // Invocation counter
  std::atomic<int> invocations_gen{0};
  std::atomic<int> invocations_kernel{0};
  std::atomic<int> invocations_reduce{0};

  void setup_empty() {
  }

  void check_empty() {
    EXPECT_EQ(1, invocations_gen);
    EXPECT_EQ(0, invocations_kernel);
    EXPECT_EQ(0, invocations_reduce);
  }

  void setup_single() {
    out = 0;
    v = vector<int>{1};
  }

  void check_single() {
    EXPECT_EQ(2, invocations_gen);
    EXPECT_EQ(1, invocations_kernel);
    EXPECT_EQ(1, invocations_reduce);

    EXPECT_EQ(2, this->out);
  }

  void setup_single_sink() {
    out = 0;
    v = vector<int>{1};
    window = 1;
    offset = 0;
  }

  void check_single_sink() {
    EXPECT_EQ(2, invocations_gen);
    EXPECT_EQ(1, invocations_reduce);
    EXPECT_EQ(1, this->out);
  }

  void setup_multiple() {
    out = 0;
    v = vector<int>{1,2,3,4,5};
  }

  void check_multiple() {
    EXPECT_EQ(6, invocations_gen);
    EXPECT_EQ(5, invocations_kernel);
    EXPECT_EQ(5, invocations_reduce);
    EXPECT_EQ(30, this->out);
  }

  void setup_multiple_sink() {
    out = 0;
    v = vector<int>{1,2,3,4,5};
    window = 1;
    offset = 1;
  }

  void check_multiple_sink() {
    EXPECT_EQ(6, invocations_gen);
    EXPECT_EQ(5, invocations_reduce);
    EXPECT_EQ(15, this->out);
  }

  void setup_window_offset() {
    out = 0;
    v = vector<int>{1,2,3,4,5,6};
    window = 2;
    offset = 1;
  }

  void check_window_offset() {
    EXPECT_EQ(7, invocations_gen);
    EXPECT_EQ(5, invocations_reduce);
    EXPECT_EQ(35, this->out);
  }

};

// Test for execution policies defined in supported_executions.h
TYPED_TEST_CASE(stream_reduce_test, executions);

// Check functionality with empty stream
TYPED_TEST(stream_reduce_test, static_empty)
{    
  this->setup_empty();
  grppi::stream_reduce(this->execution_,
    [this]() -> std::experimental::optional<int> { 
      this->invocations_gen++; 
      return {};
    },
    [this](auto v) { 
      this->invocations_kernel++; 
      return 0;
    },
    [this](int a, auto & out) { 
      this->invocations_reduce++; 
    },
    this->out
  );
  this->check_empty();
}

// Check functionality with empty stream and sink function
TYPED_TEST(stream_reduce_test, static_empty_sink)
{ 
  this->setup_empty();
  grppi::stream_reduce(this->execution_,
    [this]() -> std::experimental::optional<int> { 
      this->invocations_gen++; 
      return {};
    },
    this->window, 
    this->offset,
    std::plus<int>(),
    [this](int a) { 
      this->invocations_reduce++; 
    }
  );
  this->check_empty();
}

// Process single element 
TYPED_TEST(stream_reduce_test, static_single)
{    
  this->setup_single();
  grppi::stream_reduce(this->execution_,
    [this]() { 
      this->invocations_gen++;

      if(this->v.size() > 0){
        
        optional<int> problem(this->v.back());
        this->v.pop_back();
        return problem;

      }else{
        return optional<int> ();
      }
    },
    [this](auto v) { 
      this->invocations_kernel++; 
      return v*2;
    },
    [this](int a, auto & out) { 
      this->invocations_reduce++; 
      out += a;
    },
    this->out
  );

  this->check_single();
}

// Process single element and use sink function
TYPED_TEST(stream_reduce_test, static_single_sink)
{ 
  this->setup_single_sink();
  grppi::stream_reduce(this->execution_,
    [this]() { 
      this->invocations_gen++; 
      
      if(this->v.size() > 0){
        
        optional<int> problem(this->v.back());
        this->v.pop_back();
        return problem;

      }else{
        return optional<int> ();
      }
    },
    this->window, 
    this->offset,
    std::plus<int>(),
    [this](int a) { 
      this->invocations_reduce++;
      this->out += a;
    }
  );
  this->check_single_sink();
}

// Process multiple elements 
TYPED_TEST(stream_reduce_test, static_multiple)
{    
  this->setup_multiple();
  grppi::stream_reduce(this->execution_,
    [this]() { 
      this->invocations_gen++;

      if(this->v.size() > 0){
        
        optional<int> problem(this->v.back());
        this->v.pop_back();
        return problem;

      }else{
        return optional<int> ();
      }
    },
    [this](auto v) { 
      this->invocations_kernel++; 
      return v*2;
    },
    [this](int a, auto & out) { 
      this->invocations_reduce++; 
      out += a;
    },
    this->out
  );

  this->check_multiple();
}

// Process multiple elements and use sink function
TYPED_TEST(stream_reduce_test, static_multiple_sink)
{ 
  this->setup_multiple_sink();
  grppi::stream_reduce(this->execution_,
    [this]() { 
      this->invocations_gen++; 
      
      if(this->v.size() > 0){
        
        optional<int> problem(this->v.back());
        this->v.pop_back();
        return problem;

      }else{
        return optional<int> ();
      }
    },
    this->window, 
    this->offset,
    std::plus<int>(),
    [this](int a) { 
      this->invocations_reduce++;
      this->out += a;
    }
  );
  this->check_multiple_sink();
}

// Process multiple elements with changes in the window and offset parameters
TYPED_TEST(stream_reduce_test, static_window_offset)
{ 
  this->setup_window_offset();
  grppi::stream_reduce(this->execution_,
    [this]() { 
      this->invocations_gen++; 
      
      if(this->v.size() > 0){
        
        optional<int> problem(this->v.back());
        this->v.pop_back();
        return problem;

      }else{
        return optional<int> ();
      }
    },
    this->window, 
    this->offset,
    std::plus<int>(),
    [this](int a) { 
      this->invocations_reduce++;
      this->out += a;
    }
  );
  this->check_window_offset();
}