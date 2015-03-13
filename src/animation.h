/**
 * animation module for handling two animations and an update between both
 * @author Daniel Kagemann @corefault
 * @created 13.03.2015
 *
 */
 
 enum animationdirection { moveleft = 0, moveright, movedown, moveup };
 
 typedef struct t_animationchain {
	char			textbuffer[20];
	TextLayer		*layer;
	
	// animation 
	PropertyAnimation	*prop;	// self destruction
	
	// rects
	GRect				first_rc_src, first_rc_dest, last_rc_src, last_rc_dest;
 } animationchain;
 
 
