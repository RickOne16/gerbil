#include "viewerwindow.h"
//#include "view3d.h"
#include <qapplication.h>

int main(int argc, char **argv)
{
	if (argc < 2)
		return 1;

	// load image   
	multi_img* image = new multi_img(argv[1]);
	if (image->empty())
		return 2;

	/* compute spectral gradient */
	// log image data
	multi_img log(*image);
	log.apply_logarithm();
	multi_img* gradient = new multi_img(log.spec_gradient());

	// start gui
	QApplication app(argc, argv);
	
	// regular viewer
	ViewerWindow window(image, gradient);
	window.show();
	
/*	// fancy 3d viewer
	View3D window3d(image);
	window3d.show();
*/	
	return app.exec();
}

