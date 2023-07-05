// somebar - dwl bar
// See LICENSE file for copyright and license details.

#pragma once
#include <array>
#include <algorithm>
#include <sys/types.h>

// reads data from Reader, and passes complete lines to Consumer.
template<size_t BufSize>
class LineBuffer {
	using Iterator = typename std::array<char, BufSize>::iterator;
	std::array<char, BufSize> _buffer;
	Iterator _bufferedTo;
	Iterator _consumedTo;
	bool _discardLine {false};
public:
	LineBuffer()
		: _bufferedTo {_buffer.begin()}
		, _consumedTo {_buffer.begin()}
	{
	}

	template<typename Reader, typename Consumer>
	ssize_t readLines(const Reader& reader, const Consumer& consumer)
	{
		while (true) {
			auto bytesRead = reader(_bufferedTo, _buffer.end() - _bufferedTo);
			if (bytesRead <= 0) {
				return bytesRead;
			}
			_bufferedTo += bytesRead;
			dispatchLines(consumer);
			resetBuffer();
		}
	}
private:
	template<typename Consumer>
	void dispatchLines(const Consumer& consumer)
	{
		while (true) {
			auto separator = std::find(_consumedTo, _bufferedTo, '\n');
			if (separator == _bufferedTo) {
				break;
			}
			size_t lineLength = separator - _consumedTo;
			if (!_discardLine) {
				consumer(_consumedTo, lineLength);
			}
			_consumedTo = separator + 1;
			_discardLine = false;
		}
	}

	void resetBuffer()
	{
		size_t bytesRemaining = _bufferedTo - _consumedTo;
		if (bytesRemaining == _buffer.size()) {
			// line too long
			_discardLine = true;
			_consumedTo = _buffer.begin();
			_bufferedTo = _buffer.begin();
		} else {
			// move the last partial message to the front of the buffer, so a full-sized
			// message will fit
			std::copy(_consumedTo, _bufferedTo, _buffer.begin());
			_consumedTo = _buffer.begin();
			_bufferedTo = _consumedTo + bytesRemaining;
		}
	}
};
