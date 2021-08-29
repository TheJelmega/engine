#pragma once
#include "core/MinInclude.h"

namespace Core
{
	template<typename T>
	class IntrusiveList;

	/**
	 * Node in a intrusive linked list
	 * \tparam Owner Type that owns the node
	 * \note This should not be used by any struct/class that can be relocated by memory defragmentation
	 */
	template<typename Owner>
	class IntrusiveListNode
	{
	public:
		/**
		 * Create an intrusive list node
		 * \param memberOffset Offset to Owner type (use OffsetOf<>());
		 */
		IntrusiveListNode(usize memberOffset) noexcept;
		IntrusiveListNode(const IntrusiveListNode&) noexcept = default;
		IntrusiveListNode(IntrusiveListNode&&) noexcept = default;

		auto operator=(const IntrusiveListNode& other) noexcept -> IntrusiveListNode&;
		auto operator=(IntrusiveListNode&& other) noexcept -> IntrusiveListNode&;

		auto Get() noexcept -> Owner*;
		auto Get() const noexcept -> const Owner*;

	private:
		IntrusiveListNode* m_pNext;  ///< Pointer to next node
		usize  m_offset; ///< Offset to Owner

		friend class IntrusiveList<Owner>;
		friend class IntrusiveList<Owner>::Iterator;
	};


	/**
	 * An intrusive linked list
	 * \tparam T Underlying type
	 * \note This should not be used by any struct/class that can be relocated by memory defragmentation
	 */
	template<typename T>
	class IntrusiveList
	{
	public:

		class Iterator
		{
		public:
			Iterator() noexcept;
			Iterator(const Iterator&) noexcept = default;
			Iterator(Iterator&&) noexcept = default;

			auto operator=(const Iterator& other) noexcept;
			auto operator=(Iterator&& other) noexcept;

			auto operator++() noexcept -> Iterator;
			auto operator++(int) noexcept -> Iterator;

			auto operator->() const noexcept -> T*;
			auto operator*() const noexcept -> T&;

			auto operator+(usize count) const noexcept -> Iterator;

			auto operator==(const Iterator& other) const noexcept -> bool;
			auto operator!=(const Iterator& other) const noexcept -> bool;

		private:
			Iterator(IntrusiveListNode<T>* pNode);

			IntrusiveListNode<T>* m_pNode;
		};
		using ConstIterator = const Iterator;

	public:
		IntrusiveList() noexcept;

		/**
		 * \brief Create a IntrusiveList with the contents of another IntrusiveList
		 * \param[in] other IntrusiveList to copy
		 */
		IntrusiveList(const IntrusiveList& other) noexcept;
		/**
		 * Move another IntrusiveList into a new IntrusiveList
		 * \param[in] other IntrusiveList to move from
		 */
		IntrusiveList(IntrusiveList&& other) noexcept;
		
		auto operator=(const IntrusiveList& other) noexcept -> IntrusiveList<T>&;
		auto operator=(IntrusiveList&& other) noexcept -> IntrusiveList<T>&;
		
		/**
		 * Add an element to the IntrusiveList
		 * \param[in] val Element to add
		 */
		auto Add(IntrusiveListNode<T>& val) noexcept -> void;
		/**
		 * Add the contents of a IntrusiveList to the IntrusiveList
		 * \param[in] other IntrusiveList to add
		 */
		auto Add(IntrusiveList&& other) -> void;

		/**
		 * Insert an element after a certain location
		 * \param[in] it Iterator to position before the element to insert the element at
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto InsertAfter(Iterator& it, IntrusiveListNode<T>& val) noexcept -> Iterator;
		/**
		 * Insert a IntrusiveList into the IntrusiveList after a certain location
		 * \param[in] it Iterator to position before the element to insert elements at
		 * \param[in] other IntrusiveList to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto InsertAfter(Iterator& it, IntrusiveList&& other) noexcept -> Iterator;

		/**
		 * Add an element at the front of the IntrusiveList
		 * \param[in] val Element to insert
		 * \return Iterator to inserted element
		 */
		auto AddFront(IntrusiveListNode<T>& val) noexcept -> void;
		/**
		 * Add a IntrusiveList into the IntrusiveList at the front of the IntrusiveList
		 * \param[in] other IntrusiveList to insert
		 * \return Iterator to the first element that was inserted
		 */
		auto AddFront(IntrusiveList&& other) noexcept -> void;

		/**
		 * Clear the contents of the IntrusiveList
		 */
		auto Clear() noexcept -> void;
		/**
		 * Remove the last element from the IntrusiveList
		 */
		auto Pop() noexcept -> void;
		/**
		 * Remove the first element from the IntrusiveList
		 */
		auto PopFront() noexcept -> void;
		/**
		 * Erase an element from the IntrusiveList
		 * \param[in] it Iterator to element before the element to erase
		 */
		auto EraseAfter(Iterator& it) noexcept -> void;

		/**
		 * Reverse the elements in the list
		 * \note This does not invalidate any iterators
		 */
		auto Reverse() noexcept -> void;

		/**
		 * Get the size of the IntrusiveList
		 * \return Size of the IntrusiveList
		 */
		auto Size() const noexcept -> usize;
		/**
		 * Check if the IntrusiveList is empty
		 * \return Whether the IntrusiveList is empty
		 */
		auto IsEmpty() const noexcept ->  bool;

		/**
		 * Get the first element in the IntrusiveList
		 * \return First element in the IntrusiveList
		 * \note Only use when the IntrusiveList is not empty
		 */
		auto Front() noexcept -> T&;
		/**
		 * Get the first element in the IntrusiveList
		 * \return First element in the IntrusiveList
		 * \note Only use when the IntrusiveList is not empty
		 */
		auto Front() const noexcept -> const T&;
		/**
		 * Get the last element in the IntrusiveList
		 * \return Last element in the IntrusiveList
		 * \note Only use when the IntrusiveList is not empty
		 */
		auto Back() noexcept -> T&;
		/**
		 * Get the last element in the IntrusiveList
		 * \return Last element in the IntrusiveList
		 * \note Only use when the IntrusiveList is not empty
		 */
		auto Back() const noexcept -> const T&;

		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		auto Begin() noexcept -> Iterator;
		/**
		 * Get an iterator to the first element
		 * \return Iterator to the first element
		 */
		auto Begin() const noexcept -> ConstIterator;

		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		auto End() noexcept -> Iterator;
		/**
		 * Get an iterator to the end of the elements
		 * \return Iterator to the end of the elements
		 */
		auto End() const noexcept -> ConstIterator;

		// Overloads for 'for ( ... : ... )'
		auto begin() noexcept -> Iterator;
		auto begin() const noexcept -> ConstIterator;
		auto cbegin() const noexcept -> ConstIterator;
		auto end() noexcept -> Iterator;
		auto end() const noexcept -> ConstIterator;
		auto cend() const noexcept -> ConstIterator;

	private:
		IntrusiveListNode<T>* m_pHead;
		IntrusiveListNode<T>* m_pTail;
	};

	
}

#include "IntrusiveList.inl"