#include "q_learning.h"

void q_learning_loop(MatrixQ * matrixQ, int * last_states, int * last_action, int dx, int dy, int bird_state)
{
	/* Shift for the new state */
	shift_array(last_states, NB_SAVED_STATES);

	/* Update the new state */
	last_states[0] = findStateIndex(getCurrentState(processing_dxdy(dx), processing_dxdy(dy), processing_birdstate(bird_state)), matrixQ);

	/* Update the Q matrix */
	updateQReward(matrixQ, last_states, last_action);

	shift_array(last_action, NB_SAVED_ACTIONS);

	/* Compute the new action */ /* Reset when the bird dies */
	if(last_states[0] != -1) 
	{	
		last_action[0] = findBestAction(last_states[0], matrixQ); /* ACTION GAME */
	}
	else
	{
		init_array(last_states, NB_SAVED_STATES, -1); /* RESET GAME */
		init_array(last_action, NB_SAVED_ACTIONS, -1); /* RESET GAME */
	}
}


/*int main()
{*/
	/* Basic initialization */
	/*int i;
	srand(time(NULL));*/

	/* Array to simulate dx, dy, the bird state*/
	/*int tab_dx[18];
	int tab_dy[18];
	int tab_bird_state[18] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	for(i=0; i<18; ++i)
	{
		tab_dx[i] = randomAtMost(5);
		tab_dy[i] = randomAtMost(5);
	}*/

	/* Load Q matrix data */
	/*MatrixQ * matrixQ = loadQMatrix();*/

	/* Number of last states saved / last actions */
	/*int last_states[NB_SAVED_STATES];*/ /* [0]current_state [1]last_state */
	/*int last_action = -1;
	init_array(last_states, NB_SAVED_STATES, -1);*/

	/* main loop */
	/*while(1)
	{*/
		/* debug */
		/*i++;		
		if(i>=18) i=0;*/

		/*q_learning_loop(matrixQ, last_states, &last_action, tab_dx[i], tab_dy[i], tab_bird_state[i]);*/

		/* utils */
		/*show_matrixQ(matrixQ);*/
		/*delay(50000);*/
	/*}*/
	
	/*Save Q matrix data */
	/*saveQMatrix(matrixQ);

	return 0;
}*/