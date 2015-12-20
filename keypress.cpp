#include "globals.cpp"

//Called when a key is pressed
void handleKeyPress(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
		case 'W':
			player->goB = false;
			player->goF = true;
			break;
		case 's':
		case 'S':
			player->goF = false;
			player->goB = true;
			break;
		case 'a':
		case 'A':
			player->goR = false;
			player->goL = true;
			break;
		case 'd':
		case 'D':
			player->goL = false;
			player->goR = true;
			break;
		case ' ':
	//		player->jump();
			break;
		case 'm':
//			glutTimerFunc(0, update, 0);
			break;
		case 27: //Escape key
			exit(0);
	}
}

//Called when a key is released
void handleKeyRelease(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
		case 'W':
			player->goF = false;
			break;
		case 's':
		case 'S':
			player->goB = false;
			break;
		case 'a':
		case 'A':
			player->goL = false;
			break;
		case 'd':
		case 'D':
			player->goR = false;
			break;
	}
}

//Called when a key is pressed
void handleSpecialPress(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			player->turnD = false;
			player->turnU = true;
			break;
		case GLUT_KEY_DOWN:
			player->turnU = false;
			player->turnD = true;
			break;
		case GLUT_KEY_LEFT:
			player->turnR = false;
			player->turnL = true;
			break;
		case GLUT_KEY_RIGHT:
			player->turnL = false;
			player->turnR = true;
			break;
	}
}

//Called when a key is released
void handleSpecialRelease(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			player->turnU = false;
			break;
		case GLUT_KEY_DOWN:
			player->turnD = false;
			break;
		case GLUT_KEY_LEFT:
			player->turnL = false;
			break;
		case GLUT_KEY_RIGHT:
			player->turnR = false;
			break;
	}
}