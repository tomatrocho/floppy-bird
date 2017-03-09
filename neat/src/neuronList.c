#include "neuronList.h"

/*!
* \brief Free the specified Neuron.
* \param[out] neuron the Neuron to be freed
*/
static void freeNeuron(Neuron * neuron) {
  free(neuron);
}

/*!
* \brief Initialize the Neuron structure members to be consistent with an empty list.
* \param[out] neuron the NeuronList to be initialized
*/
void initNeuronList(NeuronList * neuron_list) {
  neuron_list->first = NULL;
  neuron_list->current = NULL;
  neuron_list->last = NULL;
}

/*!
* \brief Suppress all Neuron elements from the NeuronList.
* \param[out] neuron_list the NeuronList to be emptied
*/
void deleteNeuronList(NeuronList * neuron_list) {
  Neuron * neuron_to_delete = NULL;

  setOnFirstNeuron(neuron_list);
  while(!outOfNeuronList(neuron_list)) {
    neuron_to_delete = neuron_list->current;
    nextNeuron(neuron_list);
    freeNeuron(neuron_to_delete);
  }

  neuron_list->first = neuron_list->current = neuron_list->last = NULL;
}

/*!
* \brief Check if it is an empty NeuronList.
* \param[in] neuron_list the NeuronList to check
* \return int 1 if empty, 0 otherwise
*/
int emptyNeuronList(NeuronList * neuron_list) {
  return neuron_list->first == NULL;
}

/*!
* \brief Check if the current Neuron is not valid (ie. NULL).
* \param[in] neuron_list the NeuronList to check
* \return int 1 if the current Neuron is not valid, 0 otherwise
*/
int outOfNeuronList(NeuronList * neuron_list) {
  return neuron_list->current == NULL;
}

/*!
* \brief Set the current NeuronList on the first one.
* \param[out] neuron_list the NeuronList to be modified
*/
void setOnFirstNeuron(NeuronList * neuron_list) {
  neuron_list->current = neuron_list->first;
}

/*!
* \brief Set the current Neuron on the next one.
* \param[out] neuron_list the NeuronList to be modified
*/
void nextNeuron(NeuronList * neuron_list) {
  if (!outOfNeuronList(neuron_list))
    neuron_list->current = neuron_list->current->next;
}

/*!
* \brief Retrieve the Neuron stored in the current element.
* \param[in] neuron_list the NeuronList
* \return neuron_list the NeuronList of the current element if valid, NULL otherwise
*/
Neuron * getCurrentNeuron(NeuronList * neuron_list) {
  if (!outOfNeuronList(neuron_list))
    return neuron_list->current;

  return NULL;
}

/*!
* \brief Insert a Neuron at the tail of the NeuronList.
* \param[out] neuron_list the NeuronList
* \param[in] neuron the Neuron to be inserted
* \return int 1 if the Neuron was successfully inserted, 0 otherwise
*/
int addNeuron(NeuronList * neuron_list, Neuron * neuron) {
  if (emptyNeuronList(neuron_list))
    neuron_list->first = neuron;
  else
    neuron_list->last->next = neuron;

  neuron_list->last = neuron;

  return 1;
}

static int deleteFirstNeuron(NeuronList * neuron_list) {
  Neuron * neuron_to_delete = neuron_list->first;

  if (!emptyNeuronList(neuron_list)) {
    neuron_list->first = neuron_list->first->next;
    setOnFirstNeuron(neuron_list);

    if (neuron_list->first == NULL)
      neuron_list->last = NULL;

    freeNeuron(neuron_to_delete);
    return 1;
  }

  return 0;
}

static int deleteLastNeuron(NeuronList * neuron_list) {
  Neuron * neuron_to_delete = neuron_list->last;
  Neuron * previous_neuron = NULL;

  if (!emptyNeuronList(neuron_list)) {
    setOnFirstNeuron(neuron_list);
    if (neuron_list->current == neuron_to_delete)
      neuron_list->first = NULL;

    else {
      while (neuron_list->current != neuron_list->last) {
        previous_neuron = neuron_list->current;
        nextNeuron(neuron_list);
      }

      previous_neuron->next = NULL;
    }

    neuron_list->current = neuron_list->last = previous_neuron;

    if (neuron_list->last == NULL)
      neuron_list->first = NULL;

    freeNeuron(neuron_to_delete);
    return 1;
  }

  return 0;
}

static int deleteCurrentNeuron(NeuronList * neuron_list) {
  Neuron * previous_neuron = NULL;
  Neuron * stop = NULL;

  if (!outOfNeuronList(neuron_list)) {
    if (neuron_list->current == neuron_list->first)
      deleteFirstNeuron(neuron_list);

    else if (neuron_list->current == neuron_list->last)
      deleteLastNeuron(neuron_list);

    else {
      stop = neuron_list->current;

      setOnFirstNeuron(neuron_list);
      while (neuron_list->current != stop) {
        previous_neuron = neuron_list->current;
        nextNeuron(neuron_list);
      }

      previous_neuron->next = neuron_list->current->next;
      freeNeuron(neuron_list->current);
    }
  }
}

/*!
* \brief Delete the given Neuron from the NeuronList.
* \param[out] neuron_list the NeuronList
* \param[out] neuron the Neuron to delete
* \return int 1 if the Neuron was successfully deleted, 0 otherwise
*/
int deleteNeuron(NeuronList * neuron_list, Neuron * neuron) {
  setOnFirstNeuron(neuron_list);
  while (!outOfNeuronList(neuron_list)) {
    if (neuron_list->current == neuron) {
      deleteCurrentNeuron(neuron_list);
      return 1;
    }

    nextNeuron(neuron_list);
  }

  return 0;
}

/*!
* \brief Check if the given Neuron is stored in the NeuronList.
* \param[in] neuron_list the NeuronList
* \param[in] neuron the Neuron to be found
* \return int 1 if the Neuron was found, 0 otherwise
*/
int findNeuron(NeuronList * neuron_list, Neuron * neuron) {
  setOnFirstNeuron(neuron_list);
  while (!outOfNeuronList(neuron_list)) {
    if (neuron_list->current == neuron)
      return 1;

    nextNeuron(neuron_list);
  }

  return 0;
}