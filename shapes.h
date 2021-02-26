class Shape
{
	public:
		void createShapes();

		unsigned int currentShape();
		unsigned int rotation();
		float xTrans();
		float yTrans();
		void moveLeft();
		void moveRight();
		void moveDown();

		void nextShape();
		void rotate();

	private:
		unsigned int _currentDisplayListIndex;
		unsigned int _shapeLists[7];
		unsigned int _rotation;
		float _xTrans;
		float _yTrans;
		bool _hasDropped;

		unsigned int _minX;
		unsigned int _maxX;
		unsigned int _minY;
};


