#pragma once

namespace flounder {
	/// <summary>
	/// A extension used with standards to define a standards.
	/// </summary>
	class istandard {
	public:
		/// <summary>
		/// Run when updating the standards.
		/// </summary>
		virtual void update() = 0;
	};
}