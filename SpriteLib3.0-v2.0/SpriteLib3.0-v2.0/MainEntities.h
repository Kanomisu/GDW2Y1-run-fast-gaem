#ifndef __MAINENTITIES_H__
#define __MAINENTITIES_H__

class MainEntities
{
public:
	//Getters
	static int MainCamera();
	static int MainPlayer();
	static int MainLoading();
	static int MainCredits();

	//Setters
	static void MainCamera(int main);
	static void MainPlayer(int main);
	static void MainLoading(int main);
	static void MainCredits(int main);

private:
	static int m_mainCamera;
	static int m_mainPlayer;
	static int m_mainLoading;
	static int m_mainCredits;
};

#endif // !__MAINENTITIES_H__
