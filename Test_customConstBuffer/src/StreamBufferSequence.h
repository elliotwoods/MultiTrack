#pragma once

#include <sstream>
#include <asio.hpp>

class StreamBuffer {
public:
	class const_iterator {
	public:
		const_iterator & operator++();
		bool operator!=(const const_iterator &) const;
		asio::mutable_buffer operator*() const;
	};

	const_iterator begin() const;
	const_iterator end() const;
};

class StreamBufferSequence {
public:
	class const_iterator {
	public:
		const_iterator & operator++();
		bool operator!=(const const_iterator &) const;
		const StreamBuffer & operator*() const;
	};

	const_iterator begin() const;
	const_iterator end() const;
};