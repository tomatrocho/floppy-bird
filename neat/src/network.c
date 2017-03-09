#include "network.h"

/*!
* \brief Create a neuron
* \return Return a neuron, NULL if error
*/
Neuron * newNeuron() {
  Neuron * new_neuron = (Neuron *) malloc(sizeof(Neuron));

  if (new_neuron == (Neuron *) NULL) {
    fprintf(stderr, "Error while allocating memory for Neuron\n");
    return NULL;
  }

  new_neuron->value = 0.0;

  return new_neuron;
}

/*!
* \brief Create a connection gene
* \param[in] input the input node
* \param[in] output the output node
* \param[in] weight the weight
* \param[in] innovation the innovation number
* \param[in] enabled the enabled/disabled flag
* \return Return a connection gene, NULL if error
*/
ConnectionGene * newConnectionGene(short int input, short int output, double weight, short int innovation, unsigned char enabled) {
  ConnectionGene * new_connection_gene = (ConnectionGene *) malloc(sizeof(ConnectionGene));

  if (new_connection_gene == (ConnectionGene *) NULL) {
    fprintf(stderr, "Error while allocating memory for new ConnectionGene\n");
    return NULL;
  }

  new_connection_gene->input = input;
  new_connection_gene->output = output;
  new_connection_gene->weight = weight;
  new_connection_gene->innovation = innovation;
  new_connection_gene->enabled = enabled;

  return new_connection_gene;
}