#include "MainEntities.h"

int MainEntities::m_mainCamera = 0;
int MainEntities::m_mainPlayer = 0;
int MainEntities::m_mainLoading = 0;
int MainEntities::m_mainCredits = 0;

int MainEntities::MainCamera()
{
	return m_mainCamera;
}

int MainEntities::MainPlayer()
{
	return m_mainPlayer;
}

int MainEntities::MainLoading()
{
	return m_mainLoading;
}

int MainEntities::MainCredits()
{
	return m_mainCredits;
}


void MainEntities::MainCamera(int main)
{
	m_mainCamera = main;
}

void MainEntities::MainPlayer(int main)
{
	m_mainPlayer = main;
}

void MainEntities::MainLoading(int main)
{
	m_mainLoading = main;
}

void MainEntities::MainCredits(int main)
{
	m_mainCredits = main;
}



