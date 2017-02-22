template <int dim>
void  MatrixFreePDE<dim>::computeIntegral(double& integratedField){
	QGauss<dim>  quadrature_formula(finiteElementDegree+1);
	  FE_Q<dim> FE (QGaussLobatto<1>(finiteElementDegree+1));
	  FEValues<dim> fe_values (FE, quadrature_formula, update_values | update_JxW_values | update_quadrature_points);
	  const unsigned int   dofs_per_cell = FE.dofs_per_cell;
	  const unsigned int   n_q_points    = quadrature_formula.size();
	  std::vector<double> cVal(n_q_points);

	  typename DoFHandler<dim>::active_cell_iterator cell= this->dofHandlersSet[0]->begin_active(), endc = this->dofHandlersSet[0]->end();

	  double value = 0.0;

	  unsigned int fieldIndex;
	  fieldIndex=this->getFieldIndex("c");

	  for (; cell!=endc; ++cell) {
		  if (cell->is_locally_owned()){
	    	fe_values.reinit (cell);

	    	fe_values.get_function_values(*this->solutionSet[fieldIndex], cVal);

	    	for (unsigned int q=0; q<n_q_points; ++q){
	    		value+=(cVal[q])*fe_values.JxW(q);
	    	}
		  }
	  }

	  value=Utilities::MPI::sum(value, MPI_COMM_WORLD);

	  if (Utilities::MPI::this_mpi_process(MPI_COMM_WORLD) == 0){
	  std::cout<<"Integrated field: "<<value<<std::endl;
	  }

	  integratedField = value;
}

template <int dim>
void  MatrixFreePDE<dim>::shiftConcentration(){
  //default trivial implementation.
}
