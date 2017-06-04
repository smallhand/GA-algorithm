class parent{
	public:
		int* genes;
		int fitness;
		~parent(){
			delete [] genes;
		}
};
