#define Mapplyparam3_to_point(_fx, _fy, _col, _row, _dx, _mxx, _mxy, _dy, _myy, _myx) \
   *_fx = (_mxx * _col) + (_mxy * _row) + _dx; \
   *_fy = (_myy * _row) + (_myx * _col) + _dy
