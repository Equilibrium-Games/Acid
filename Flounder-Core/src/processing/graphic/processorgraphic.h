#pragma once

#include "../../framework/framework.h"

#include "requestgraphic.h"
#include "../iprocessor.h"
#include "../queue.h"

namespace flounder
{
	/// <summary>
	/// A extension that is responsible for processing OpenGL requests.
	/// </summary>
	class processorgraphic :
		public iprocessor
	{
	private:
		static const double MAX_TIME_MILLIS;

		queue<requestgraphic*> *m_queue;
	public:
		/// <summary>
		/// Creates a new opengl processor.
		/// </summary>
		processorgraphic();

		/// <summary>
		/// Deconstructor for the opengl processor.
		/// </summary>
		~processorgraphic();

		void update() override;

		void addRequestToQueue(irequest *request) override;

		/// <summary>
		/// Completes all requests left in queue.
		/// </summary>
		void completeAllRequests();
	};
}
