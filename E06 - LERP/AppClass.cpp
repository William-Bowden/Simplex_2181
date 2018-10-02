#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "William Bowden - wjb5377@rit.edu";

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(vector3(5.0f, 3.0f, 15.0f), ZERO_V3, AXIS_Y);

	m_pModel = new Simplex::Model();
	m_pModel->Load("Sorted\\WallEye.bto");

	m_stopsList.push_back(vector3(-4.0f, -2.0f, 5.0f));
	m_stopsList.push_back(vector3(1.0f, -2.0f, 5.0f));

	m_stopsList.push_back(vector3(-3.0f, -1.0f, 3.0f));
	m_stopsList.push_back(vector3(2.0f, -1.0f, 3.0f));

	m_stopsList.push_back(vector3(-2.0f, 0.0f, 0.0f));
	m_stopsList.push_back(vector3(3.0f, 0.0f, 0.0f));

	m_stopsList.push_back(vector3(-1.0f, 1.0f, -3.0f));
	m_stopsList.push_back(vector3(4.0f, 1.0f, -3.0f));

	m_stopsList.push_back(vector3(0.0f, 2.0f, -5.0f));
	m_stopsList.push_back(vector3(5.0f, 2.0f, -5.0f));

	m_stopsList.push_back(vector3(1.0f, 3.0f, -5.0f));
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	// Draw the model
	m_pModel->PlaySequence();

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	//calculate the current position
	vector3 v3CurrentPos;


	//std::cout << v3CurrentPos.x << " " << targetPos.x << (targetPos - v3CurrentPos).x << std::endl;



	//your code goes here
	static int currentIndex = 0;
	static int pointsReached = 0;
	float speed = 1.0f; // time it takes to get from point to point
	static vector3 startPos;
	vector3 targetPos = m_stopsList[currentIndex];

	// if we're going to the first point
	if (currentIndex == 0) {
		// set the currentPos to the startPos
		v3CurrentPos = startPos;
	}
	// otherwise, use the previous stop
	else {
		v3CurrentPos = m_stopsList[currentIndex-1];
	}

	// calculate the percentage complete the movement is from it's start time and travel time
	float fPercentage = MapValue(fTimer, speed * pointsReached, speed + (speed * pointsReached), 0.0f, 1.0f);

	// lerp the current pos
	v3CurrentPos = glm::lerp(v3CurrentPos, targetPos, fPercentage);

	// if we have reached the target
	if (fPercentage >= 1.0f) {
		// increment points reached
		pointsReached++;

		// grab the next stop (wrapping around when we get to the last stop)
		if (currentIndex == m_stopsList.size()-1){
			currentIndex = 0;
			startPos = m_stopsList[m_stopsList.size() - 1];
		}
		else {
			currentIndex++;
			v3CurrentPos = m_stopsList[currentIndex - 1];
		}
	}
	//-------------------




	matrix4 m4Model = glm::translate(v3CurrentPos);
	m_pModel->SetModelMatrix(m4Model);

	m_pMeshMngr->Print("\nTimer: ");//Add a line on top
	m_pMeshMngr->PrintLine(std::to_string(fTimer), C_YELLOW);

	// Draw stops
	for (uint i = 0; i < m_stopsList.size(); ++i)
	{
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_stopsList[i]) * glm::scale(vector3(0.05f)), C_GREEN, RENDER_WIRE);
	}

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pModel);
	//release GUI
	ShutdownGUI();
}
