#pragma once

class FlatBinary;

class Executor {
public:
	Executor () = default;

	bool RunBinary (std::shared_ptr<FlatBinary> binary);
};
