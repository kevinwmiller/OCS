/*Copyright (c) <2014> Kevin Miller - KevM1227@gmail.com

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#ifndef PACKEDARRAY_H
#define PACKEDARRAY_H

#include <vector>
#include <stack>
#include <utility>
#include <initializer_list>
#include <iostream>
#include <limits>

#include <fstream>

using Index = uint64_t;

/** \brief Polymorphic class for PackedArrays
 *
 */
class BasePackedArray
{
    public:
        virtual ~BasePackedArray() {}
        virtual Index createCopy(Index) = 0;
        virtual Index remove(Index) = 0;
        virtual void clear() = 0;
        virtual Index size() const = 0;
        virtual bool isValid(Index) const = 0;
        static const Index INVALID_INDEX = std::numeric_limits<Index>::max();
};

/** \brief A wrapper around a vector trades space efficiency for a constant time remove function. All other
 *         indexes are unaffected through the use of lookup tables.
 *
 */
template <typename T>
class PackedArray : public BasePackedArray
{
    public:
        PackedArray() {std::ofstream out("out.log");out.close();}

        PackedArray (std::initializer_list<T> iList)
        {
            for(auto it = iList.begin(); it != iList.end(); ++it)
                addItem(*it);
        }

        PackedArray(std::size_t numberToReserve)
        {
            if(numberToReserve > 0)
            {
                elements.reserve(numberToReserve);
                elementIds.reserve(numberToReserve);
                reverseLookupList.reserve(numberToReserve);
            }
        }

        ~PackedArray()
        {}

        typename std::vector<T>::iterator begin()
        {
            return elements.begin();
        }

        typename std::vector<T>::const_iterator begin() const
        {
            return elements.cbegin();
        }

        typename std::vector<T>::iterator end()
        {
            return elements.end();
        }

        typename std::vector<T>::const_iterator end() const
        {
            return elements.cend();
        }

        T& operator[](Index idx)
        {
            return elements[ elementIds[idx] ];
        }

        const T& operator[](Index idx) const
        {
            return elements[ elementIds[idx] ];
        }

        Index size() const
        {
            return elements.size();
        }

        //Get the Id of an element at a given position in the actual list
        Index getId(Index idx)
        {
            if (idx < size()) {
                std::cout << "Returning " << reverseLookupList[idx] << "\n";
                std::cout << "Actual index " << elementIds[reverseLookupList[idx]] << "\n";
                return reverseLookupList[idx];
            }
            return INVALID_INDEX;
        }

        Index addItem(const T& item)
        {
            Index newIdx;
            elements.push_back(item);

            if(availableIds.size() > 0)
            {
               // std::cout << "Free indexes\n";
                newIdx = availableIds.top();
                elementIds[newIdx] = size() - 1;
                //reverseLookupList[size() - 1] = newIdx;
                reverseLookupList.push_back(newIdx);
                availableIds.pop();
            }
            else
            {
               // std::cout << "No free indexes\n";
                newIdx = elements.size() - 1;
                elementIds.push_back(newIdx);
                reverseLookupList.push_back(newIdx);
            }
            return newIdx;
        }

        template<typename ... Args>
        Index emplaceItem(Args&& ... args)
        {
            T newItem(std::forward<Args>(args) ... );
            return addItem(newItem);
        }

        Index createCopy(Index indexToCopy)
        {
            if(isValid(indexToCopy))
                return addItem(elements[indexToCopy]);
            return INVALID_INDEX;
        }

        bool isValid(Index idx) const
        {
            return (idx < elementIds.size() && elementIds[idx] != INVALID_INDEX);
        }

        Index remove(Index idx)
        {
            Index swappedId = INVALID_INDEX;

            std::ofstream out("out.log", std::ios::app);
            std::cout << "Removing " << idx << "\n";
            if(isValid(idx))
            {
                std::cout << "Valid " << idx <<"\nBefore\n";

                //printVector(out, elements , "Elements");
                printVector(std::cout, elementIds, "ElementIds");
                printVector(std::cout, reverseLookupList, "Reverse");
                Index indexToRemove = elementIds[idx];
                swappedId = reverseLookupList[size() - 1];

                elementIds[swappedId] = indexToRemove;
                reverseLookupList[indexToRemove] = swappedId;

                std::swap(elements[indexToRemove], elements[size() - 1]);
                //elements[indexToRemove] = elements[size() - 1];
                elements.pop_back();
                reverseLookupList.pop_back();
                elementIds[idx] = INVALID_INDEX;

                std::cout << "After\n";
                //printVector(out, elements , "Elements");
                printVector(std::cout, elementIds, "ElementIds");
                printVector(std::cout, reverseLookupList, "Reverse");
                availableIds.push(idx);
            }
            out.close();
            return swappedId;
        }

        template<typename F>
        void printVector(std::ostream& out, std::vector<F>& v, const std::string& label)
        {
            out << label << "\n";
            for (auto& i : v)
                out << i << " ";
            out << "\n";
        }

        void clear()
        {
            elements.clear();
            reverseLookupList.clear();
            elementIds.clear();

            while(availableIds.size() > 0)
                availableIds.pop();
        }

    private:

        std::vector<T> elements;
        std::vector<Index> elementIds;
        std::vector<Index> reverseLookupList;
        std::stack<Index> availableIds;

};

template<typename T>
std::ostream& operator<<(std::ostream& out, const PackedArray<T>& arry)
{
    for(auto& i : arry)
        out << i << " ";
    return out;
}

#endif // PACKEDARRAY_H
