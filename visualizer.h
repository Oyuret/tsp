#pragma once
TSP* tsp;

#include <windows.h>   // Standard Header For Most Programs
#include <gl/gl.h>     // The GL Header File
#include <gl/glut.h>   // The GL Utility Toolkit (Glut) Header
#pragma comment(lib,"glut32.lib")

float window_width;
float window_height;

void glVertex( const Node& n ) {
  glVertex2f(n.x,n.y);
}

void gl_init ( )     // Create Some Everyday Functions
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
  glClearDepth(1.0f);									// Depth Buffer Setup
  glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
  glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
}

void gl_draw_square(float x, float y, float s = 0.2f) {
  glBegin(GL_QUADS);					// Draw A Quad
  glVertex3f( x-s, y+s, 0.0f );	// Top Left
  glVertex3f( x+s, y+s, 0.0f );	// Top Right
  glVertex3f( x+s, y-s, 0.0f );	// Bottom Right
  glVertex3f( x-s, y-s, 0.0f );	// Bottom Left
  glEnd();
}

void gl_keyboard(unsigned char key, int x, int y) {
  if (key==27)
    exit(0);
  else
    Sleep(1);
}

bool firstTime=true;
void gl_display ( void )	// Create The Display Function
{

  // Setup a 2D projection
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho (0, window_width, 0, window_height, 0, 1);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();

  // Move center of nodes to the middle of the screen
  float margin = 50.0f;
  float sy = window_height / (100+margin);
  float sx = window_width  / (100+margin);
  glScalef(sx,sy,1.0f);
  glTranslatef(margin/2, margin/2, 0);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer


  glBegin(GL_LINES);
  std::vector<int>& tour = tsp->get_tour();
  std::vector<Node>& nodes = tsp->get_nodes();

  for(size_t i=0; i<tour.size()-1; ++i){
    glColor3f(1,0,0);	glVertex2f(nodes[tour[i+0]].x,nodes[tour[i+0]].y);	// From A
    glColor3f(0,0,1);	glVertex2f(nodes[tour[i+1]].x,nodes[tour[i+1]].y);	// To B
  }

  // Add the last line to create a cycle when all nodes are assigned
  if(tour.size() == nodes.size()){
    glColor3f(1,0,0);	glVertex2f(nodes[tour[tour.size()-1]].x,nodes[tour[tour.size()-1]].y);	// From last to first (to create a cycle)
    glColor3f(0,0,1);	glVertex2f(nodes[tour[      0      ]].x,nodes[tour[      0      ]].y);	// From last to first (to create a cycle)
  }
  glEnd();

  for(size_t i=0; i<nodes.size(); ++i){

#ifdef DRAW_NODE_NUMBERS
    // Draw the number of the nodes
    glColor3f(0.0f, 1.0f, 1.0f);
    if(i<10){
      glRasterPos2f(nodes[i].x-0.4, nodes[i].y-0.8);
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'0'+i%10);
    }
    else if(i<100){
      glRasterPos2f(nodes[i].x-2*0.4, nodes[i].y-0.8);
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'0'+i/10);
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'0'+i%10);
    }
    else if(i<1000){
      glRasterPos2f(nodes[i].x-3*0.4, nodes[i].y-0.8);
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'0'+i/100);
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'0'+(i/10)%10);
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'0'+i%10);
    }
#else
    glColor3f(1.0f,1.0f,0.0f);
    gl_draw_square(nodes[i].x,nodes[i].y, 0.2f+(i==0)*0.5f);
#endif
  }


  glutSwapBuffers ( );	// Swap The Buffers To Not Be Left With A Clear Screen

}

void gl_reshape ( int w, int h )	// Create The Reshape Function (the viewport)
{

  window_width = w;
  window_height = h;
  glViewport		( 0, 0, w, h );
  glMatrixMode	( GL_PROJECTION );		// Select The Projection Matrix
  glLoadIdentity	( );					// Reset The Projection Matrix
  if ( h==0 )								// Calculate The Aspect Ratio Of The Window
    gluPerspective	( 80, ( float ) w, 1.0, 5000.0 );
  else
    gluPerspective	( 80, ( float ) w / ( float ) h, 1.0, 5000.0 );
  glMatrixMode	( GL_MODELVIEW );		// Select The Model View Matrix
  glLoadIdentity	( );					// Reset The Model View Matrix

}

void use_opengl( int argc, char** argv , TSP* touren){
  tsp = touren;

  glutInit			( &argc, argv );
  gl_init				();

  glutInitDisplayMode	( GLUT_RGB | GLUT_DOUBLE ); // Display Mode
  glutInitWindowSize	( 1000, 1000 );				// If glutFullScreen wasn't called this is the window size

  glutCreateWindow	( "Visualizer" );			// Window Title (argv[0] for current directory as title)
  glutDisplayFunc		( gl_display );				// Matching Earlier Functions To Their Counterparts
  glutReshapeFunc		( gl_reshape );
  glutKeyboardFunc    ( gl_keyboard );

  glutMainLoop		( );						// Initialize The Main Loop
}