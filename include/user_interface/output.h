#pragma once

#include <iostream>

namespace output {

   template<typename T> inline void print(const T& val) {
      std::cout << val;
   }

   class ProgressBar {
   private:
      char _filler{ '=' };
      char _head{ '>' };
      char _begin{ '[' };
      char _end{ ']' };
      int _width{ 80 };
      bool _showpercent{ true };
      std::string _prefix{};

   public:
      ProgressBar() {}
      ProgressBar(const std::string& prefix) : _prefix{ prefix } {}
      ProgressBar(char filler, char head, char begin, char end, int width, bool show, std::string& prefix) :
         _filler{ filler }, _head{ head }, _begin{ begin }, _end{ end }, _width{ width }, _showpercent{ show }, _prefix{ prefix } {
      }

      void update(float progress) {
         int filledWidth = static_cast<int>(progress * _width);

         std::cout << _prefix << _begin;
         for (int i = 0; i < _width; ++i) {
            if (i < filledWidth)
               std::cout << _filler;
            else if (i == filledWidth) std::cout << _head;
            else
               std::cout << ' ';
         }
         std::cout << _end << ' ';
         if (_showpercent) std::cout << int(progress * 100.0) << "%";
         if (int(progress * 100.0) == 100) std::cout << '\n';
         else std::cout << '\r';
         std::cout.flush();
      }
   };
}
