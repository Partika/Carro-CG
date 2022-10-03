/* lightPosition.c - demonstrates how the light position is affected by
 *             the modelview matrix
 *
 *  Left Mouse Button       - change incidence and azimuth angles
 *  Middle Mousebutton      - change the twist angle based on
 *                            horizontal mouse movement
 *  Right Mousebutton       - zoom in and out based on vertical
 *                            mouse movement
 *  <l> key                 - toggle light binding
 *  Escape key              - exit the program
 */

#include <GL/glut.h>    /* includes gl.h, glu.h */

#include <math.h>
#include <stdio.h>

/*  Function Prototypes  */

void  initgfx( );
void  drawScene( );
void  reshape( GLsizei, GLsizei );
void  keyboard( GLubyte, GLint, GLint );
void  mouse( GLint, GLint, GLint, GLint );
void  motion( GLint, GLint );

void resetView( );
void polarView( GLfloat, GLfloat, GLfloat, GLfloat );
void printHelp( char * );

/* Global Definitions */

#define KEY_ESC     27      /* ascii value for the escape key */

/* Global Variables */

enum lighttypes { EYE_LIGHT, SCENE_LIGHT };
static GLint        lightpos;

enum actions { MOVE_EYE, TWIST_EYE, ZOOM, MOVE_NONE };
static GLint        action;

static GLdouble     xStart = 0.0, yStart = 0.0;

static GLfloat      fovy, nearClip, farClip;
static GLfloat      distance, twistAngle, incAngle, azimAngle;

int main( int argc, char *argv[] )
{
    GLsizei width, height;

    glutInit( &argc, argv );

    width = glutGet( GLUT_SCREEN_WIDTH );
    height = glutGet( GLUT_SCREEN_HEIGHT );
    glutInitWindowPosition( width / 4, height / 4 );
    glutInitWindowSize( (width / 2) - 4, height / 2 );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutCreateWindow( argv[0] );

    initgfx();

    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc( reshape );
    glutDisplayFunc( drawScene );

    printHelp( argv[0] );

    glutMainLoop();

    return 0;
}

void printHelp( char *progname )
{
    fprintf(stdout, "\n%s - demonstrate how the modelview matrix "
         "affects the light position\n\n"
         "Left Mousebutton    - move eye position\n"
         "Middle Mousebutton     - change twist angle\n"
         "Right Mousebutton      - move up / down to zoom in / out\n"
         "<l> Key          - toggle light binding\n"
         "Escape Key          - exit the program\n\n",
         progname);

    if ( lightpos == EYE_LIGHT )
         printf("Light position attached to Viewpoint\n");
    else if ( lightpos == SCENE_LIGHT )
         printf("Light position fixed in scene\n");
}

void initgfx( )
{
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glEnable( GL_DEPTH_TEST );

    fovy = 60.0;    /* field of view in Y */
    nearClip = 3.0;     /* Near clipping plane location */
    farClip  = 12.0;    /* Far clipping plane location */

    resetView();

    lightpos = EYE_LIGHT;

    /* Turn on a default light */
    glEnable( GL_LIGHT0 );
}

void keyboard( GLubyte key, GLint x, GLint y )
{
    switch (key) {
    case 'l':    /* toggle light position */
         if ( lightpos == EYE_LIGHT ) {
              lightpos = SCENE_LIGHT;
              printf("Light position fixed in scene\n");
         } else if ( lightpos == SCENE_LIGHT ) {
              lightpos = EYE_LIGHT;
              printf("Light position attached to viewpoint\n");
         }
         glutPostRedisplay();
         break;
    case KEY_ESC:   /* Exit when the Escape key is pressed */
         exit(0);
    }
}

void mouse( GLint button, GLint state, GLint x, GLint y )
{
    static GLint buttons_down = 0;

    if (state == GLUT_DOWN) {
         switch (button) {
         case GLUT_LEFT_BUTTON:
              action = MOVE_EYE;
              break;
         case GLUT_MIDDLE_BUTTON:
              action = TWIST_EYE;
              break;
         case GLUT_RIGHT_BUTTON:
              action = ZOOM;
              break;
         }

         /* Update the saved mouse position */
         xStart = x;
         yStart = y;
    } else {
         if (--buttons_down == 0)
              action = MOVE_NONE;
    }
}

void motion( GLint x, GLint y )
{
    switch (action) {
    case MOVE_EYE:
         /* Adjust the eye position based on the mouse position */
         azimAngle += (GLdouble) (x - xStart);
         incAngle -= (GLdouble) (y - yStart);
         break;
    case TWIST_EYE:
         /* Adjust the eye twist based on the mouse position */
         twistAngle = fmod(twistAngle+(x - xStart), 360.0);
         break;
    case ZOOM:
         /* Adjust the eye distance based on the mouse position */
         distance -= (GLdouble) (y - yStart)/10.0;
         break;
    default:
         printf("unknown action %d\n", action);
    }

    /* Update the stored mouse position for later use */
    xStart = x;
    yStart = y;

    glutPostRedisplay();
}

void resetView( )
{
    distance = nearClip + (farClip - nearClip) / 2.0;
    twistAngle = 0.0;    /* rotation of viewing volume (camera) */
    incAngle = 60.0;
    azimAngle = 0.0;
    fovy = 60.0;    /* Field of view in Y angle */
}

void reshape( GLsizei width, GLsizei height )
{
    GLdouble     aspect;

    glViewport( 0, 0, width, height );

    aspect = (GLdouble) width / (GLdouble) height;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( fovy, aspect, nearClip, farClip );
    glMatrixMode( GL_MODELVIEW );
}

void polarView( GLfloat distance, GLfloat azimuth, GLfloat incidence, GLfloat twist)
{
    glTranslatef( 0.0, 0.0, -distance);
    glRotatef( -twist, 0.0, 0.0, 1.0);
    glRotatef( -incidence, 1.0, 0.0, 0.0);
    glRotatef( -azimuth, 0.0, 0.0, 1.0);
}

void XYZaxes( )
{
    glPushMatrix ();

    glLineWidth (2.0);

    glBegin (GL_LINES);
    glColor3f (1,0,0); // X axis is red.
    glVertex3f (0,0,0);
    glVertex3f (1,0,0);

    glColor3f (0,1,0); // Y axis is green.
    glVertex3f (0,0,0);
    glVertex3f (0,1,0);

    glColor3f (0,0,1); // z axis is blue.
    glVertex3f (0,0,0);
    glVertex3f (0,0,1);

    glEnd();

    glPopMatrix ();
}

void drawScene( )
{
    /* Define a few materials properties */
    GLfloat   redAmbient[] = { 0.3, 0.1, 0.1, 1.0 };
    GLfloat   redDiffuse[] = { 1.0, 0.0, 0.0, 1.0 };
    GLfloat   blueAmbient[] = { 0.1, 0.1, 0.3, 1.0 };
    GLfloat   blueDiffuse[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat   yellowDiffuse[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat   yellowEmission[] = { 0.6, 0.6, 0.0, 1.0 };
    GLfloat   defaultEmission[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat   whiteSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat   greenSpecular[] = { 0.0, 1.0, 0.0, 1.0 };
    GLfloat   defaultSpecular[] = { 0.0, 0.0, 0.0, 1.0 };

    /* infinite light */
    GLfloat   lightPosition[] = { 0.0, 0.0, 1.0, 0.0 };

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushMatrix();

       if ( lightpos == EYE_LIGHT ) {
           /* By setting the light position before the viewing
            * transformation, the light moves with the eye.
            * (In other words, it is always in the same
            * position relative to the viewpoint.)
            */
           glLightfv( GL_LIGHT0, GL_POSITION, lightPosition);
        }

        polarView( distance, azimAngle, incAngle, twistAngle );

        XYZaxes();

        if ( lightpos == SCENE_LIGHT ) {
           /* By setting the light positions after the
            * viewing transformation, the light(s) will
            * be fixed in the scene.
            */
           glLightfv( GL_LIGHT0, GL_POSITION, lightPosition );
        }

        glEnable( GL_LIGHTING );

        glMaterialfv( GL_FRONT, GL_EMISSION, defaultEmission );

        /* Set properties for a shiny red material,
       * with a green highlight */
        glMaterialfv( GL_FRONT, GL_AMBIENT, redAmbient );
        glMaterialfv( GL_FRONT, GL_DIFFUSE, redDiffuse );
        glMaterialfv( GL_FRONT, GL_SPECULAR, greenSpecular );
        glMaterialf( GL_FRONT, GL_SHININESS, 128.0 );
        glPushMatrix();
           glTranslatef( -2.0, 1.5, 0.0 );
           glutSolidSphere( 0.7, 31, 31 );
        glPopMatrix();

        /* Set properties for a dull blue material with
       *   a small white highlight */
        glMaterialfv( GL_FRONT, GL_AMBIENT, blueAmbient );
        glMaterialfv( GL_FRONT, GL_DIFFUSE, blueDiffuse );
        glMaterialfv( GL_FRONT, GL_SPECULAR, whiteSpecular );
        glMaterialf( GL_FRONT, GL_SHININESS, 20.0 );
        glPushMatrix();
           glTranslatef( 2.5, 0.0, 0.0 );
           glutSolidTorus( 0.25, 0.75, 16, 31 );
        glPopMatrix();

        /* Set properties for a yellow glowing material */
        glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellowDiffuse);
        glMaterialfv( GL_FRONT, GL_EMISSION, yellowEmission );
        glMaterialfv( GL_FRONT, GL_SPECULAR, defaultSpecular );

        glPushMatrix();
           glTranslatef( 0.0, 2.0, 2.0 );
           glutSolidCube( 0.5 );
        glPopMatrix();

        glDisable( GL_LIGHTING );

    glPopMatrix();
    glutSwapBuffers();
}