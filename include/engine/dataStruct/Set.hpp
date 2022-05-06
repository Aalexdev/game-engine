#pragma once

#include "../core.hpp"
#include "../pch.hpp"



namespace engine{

	template<typename T>
	class Set{
		public:
			struct Node{
				Ref<Node> next = nullptr;
				Ref<Node> back = nullptr;

				T value;

				Node(T x) : value{x} {}
			};

			struct iterator{
				Ref<Node> node = nullptr;

				iterator &operator++(int){
					if (node){
						node = node->next;
						return *this;
					}
					throw std::runtime_error("invalid iterator");
				}

				iterator &operator--(int){
					if (node){
						node = node->back;
						return *this;
					}
					throw std::runtime_error("invalid iterator");
				}

				T operator->(){
					if (node){
						return node->value;
					}
					throw std::runtime_error("invalid iterator");
				}

				T operator*(){
					if (node){
						return node->value;
					}
					throw std::runtime_error("invalid iterator");
				}

				bool operator!=(const iterator &other){
					if (node == nullptr) return false;
					return other.node != node;
				}

				bool operator==(const iterator &other){
					return other.node == node;
				}

				operator bool(){
					return node != nullptr;
				}

				iterator(const Ref<Node>& node) : node{node}{}
				iterator() = default;
			};

			Set(){}
			Set(std::initializer_list<T> list){
				for (auto e : list){
					pushBack(e);
				}
			}

			static Set<T> copy(Set<T> set){
				Set newSet;

				Set::iterator it = set.begin();
				while (it){
					newSet.pushBack(*it);
					it++;
				}

				return newSet;
			}

			/**
			 * @brief get if the set is currently empty
			 */
			bool empty(){
				return _begin == nullptr;
			}

			/**
			 * @brief get the count of elements there is in the set
			 * @return uint32_t 
			 */
			uint32_t size(){
				return length;
			}

			/**
			 * @brief push an element at the end of the set
			 * @param value the value to push
			 */
			void pushBack(T value){
				if (map.find(value) != map.end()){
					if (size() == 1) return;
					erase(value);
				}

				Ref<Node> node = createRef<Node>(value);
				map[value] = node;

				node->back = nullptr;
				node->next = nullptr;

				if (empty()){
					_begin = node;
					_end = node;

				} else {
					_end->next = node;
					node->back = _end;
					_end = node;
				}

				length++;
			}

			/**
			 * @brief push an element at the begin of the set
			 * @param value the value to push
			 */
			void pushFront(T value){
				if (map.find(value) != map.end()){
					if (size() == 1) return;
					erase(value);
				}

				Ref<Node> node = createRef<Node>(value);
				map[value] = node;

				node->back = nullptr;
				node->next = nullptr;

				if (empty()){
					_begin = node;
					_end = node;

				} else {
					_begin->back = node;
					node->next = _begin;
					_begin = node;
				}
				
				length++;
			}
			
			/**
			 * @brief remove the first element of the set
			 */
			void popFront(){
				if (empty()) return;
				
				if (_begin->next){
					_begin->next->back = nullptr;
					map.erase(_begin->value);
					_begin = _begin->next;

				} else {
					map.erase(_begin->value);
					_begin = nullptr;
				}
				length --;
			}

			/**
			 * @brief remove the last element of the set
			 */
			void popBack(){
				if (empty()) return;

				if (_end->back){
					_end->back->next = nullptr;
					map.erase(_end->value);
					_end = _end->back;

				} else {
					map.erase(_end->value);
					_end = nullptr;
				}
				length --;
			}

			/**
			 * @brief remove the given element from the set
			 * @param value the value to remove
			 */
			void erase(T value){
				if (map.find(value) == map.end()) return;

				Ref<Node> node = map[value];

				if (node->next && node->back){
					Ref<Node> back = node->back;
					Ref<Node> next = node->next;

					next->back = back;
					back->next = next;

				} else if (node->next){
					Ref<Node> next = node->next;
					next->back = nullptr;
					_begin = next;

				} else if (node->back){
					Ref<Node> back = node->back;
					back->next = nullptr;
					_end = back;

				}
				
				map.erase(value);
				length --;
			}

			/**
			 * @brief push the value behind the given element
			 * @param value the element to push
			 */
			void insertBack(T pos, T value){
				if (map.find(pos) == map.end()) return;

				if (map.find(value) != map.end()){
					erase(value);
				}

				Ref<Node> &dest = map[pos];

				if (dest->next){
					Ref<Node> node = createRef<Node>(value);
					node->next = nullptr;
					node->back = nullptr;
					map[value] = node;
				
					Ref<Node> next = dest->next;
					next->back = node;
					dest->next = node;

					node->back = dest;
					node->next = next;
				} else {
					pushBack(value);
				}
			}

			/**
			 * @brief push the value after the given element
			 * @param value the element to push
			 */
			void insertFront(T pos, T value){
				if (map.find(pos) == map.end()) return;
				
				if (map.find(value) != map.end()){
					erase(value);
				}

				Ref<Node> &dest = map[pos];

				if (dest->back){
					Ref<Node> node = createRef<Node>(value);
					node->next = nullptr;
					node->back = nullptr;
					map[value] = node;
				
					Ref<Node> back = dest->back;
					dest->back = node;
					back->next = node;

					node->next = dest;
					node->back = back;
				} else {
					pushFront(value);
				}
			}

			/**
			 * @brief swap the given element with the element next to it
			 * @param value the element to swap
			 */
			void swapForward(T value){
				if (map.find(value) == map.end()) return;
				Ref<Node> node = map[value];

				if (!node->next) return;
				Ref<Node> next = node->next;
				Ref<Node> back = node->back;

				node->next = next->next;
				if (next->next){
					next->next->back = node;
				} else {
					_end = node;
				}
				
				next->back = back;

				if (back){
					back->next = next;
				} else {
					_begin = next;
				}

				next->next = node;
				node->back = next;
			}

			/**
			 * @brief swap the given element with the element behind it
			 * @param value the element to swap
			 */
			void swapBackward(T value){
				if (map.find(value) == map.end()) return;
				Ref<Node> node = map[value];

				if (!node->back) return;
				Ref<Node> back = node->back;
				Ref<Node> next = node->next;

				node->back = back->back;
				if (back->back){
					back->back->next = node;
				} else {
					_begin = node;
				}

				back->next = next;

				if (next){
					next->back = back;
				} else {
					_end = back;
				}

				back->back = node;
				node->next = back;

			}

			/**
			 * @brief the iterator to the first element of the set
			 * @return iterator 
			 */
			iterator begin(){
				return iterator(_begin);
			}

			/**
			 * @brief the iterator to the last element of the set
			 * @return iterator 
			 */
			iterator end(){
				return iterator(_end);
			}

			/**
			 * @brief remove all elements from the set
			 */
			void clear(){
				iterator it = begin();

				while (it){
					Ref<Node> node = it.node;

					if (node->back){
						node->back->next = nullptr;
					}
					node->back = nullptr;

					it++;
				}

				_end = nullptr;
				_begin = nullptr;
				map.clear();
			}

		private:
			uint32_t length = 0;
			Ref<Node> _begin = nullptr;
			Ref<Node> _end = nullptr;
			std::unordered_map<T, Ref<Node>> map;
	};
	
	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, Set<T> s){
		os << '[';
		auto it = s.begin();

		while (it){
			os << *it;
			it++;

			if (it){
				os << ", ";
			}
		}

		os << "]";

		return os;
	}
}