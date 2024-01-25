#include"MyDirectX.h"

//#define OpenLog 1
//#define CodeManagerOpenMessagebox 1

//max size -> 1e4
template<class MESSAGE>class CODE_MANAGER {

#ifndef OpenLog
#define OpenLog 0
#endif
#ifndef CodeManagerOpenMessagebox
#define CodeManagerOpenMessagebox 0
#endif
public:
	vector<MESSAGE>code_concrete;
	// ----------------------------------------------------------------
	// private here
private:
#if OpenLog
	ofstream LogOut;
#endif
	const int max_code_store_size = (int)1e4 + 5;
	struct MESSAGE_SIMPLIFIED {
		MESSAGE_SIMPLIFIED* l, * r;
		int tol;
		MESSAGE_SIMPLIFIED(int _tol) :l(NULL), r(NULL), tol(_tol) {};
	};
	int codesize;
	MESSAGE EmptyMessage;
	MESSAGE_SIMPLIFIED* first_plug;
	stack<MESSAGE_SIMPLIFIED*>iterstackx;
	stack<int>iterstackl, iterstackr, iterstackstate;
	void __cdmg_inner_iteropt_push(int l, int r, MESSAGE_SIMPLIFIED* x, int state) {
		iterstackl.push(l);
		iterstackr.push(r);
		iterstackx.push(x);
		iterstackstate.push(state);
	}
	void __cdmg_inner_iteropt_pop() {
		iterstackl.pop();
		iterstackr.pop();
		iterstackx.pop();
		iterstackstate.pop();
	}
	void __cdmg_inner_iteropt_get(int& l, int& r, MESSAGE_SIMPLIFIED*& x, int& state) {
		if (!iterstackstate.size())return;
		l = iterstackl.top();
		r = iterstackr.top();
		x = iterstackx.top();
		state = iterstackstate.top();
	}
	void __cdmg_inner_iterator_next() {
		int l, r, state;
		MESSAGE_SIMPLIFIED* x;
		__cdmg_inner_iteropt_get(l, r, x, state);
		if (l == r) {
			__cdmg_inner_iteropt_pop();
			__cdmg_inner_iteropt_get(l, r, x, state);
		}
		while (l != r && iterstackstate.size()) {
			//			cout << l << ' ' << r << ' ' << state << endl;
			if (state == 2) {
				__cdmg_inner_iteropt_pop();
			}
			else if (state == 0) {
				int mid = (l + r) / 2;
				iterstackstate.pop();
				iterstackstate.push(1);
				if ((*x).l != NULL && (*((*x).l)).tol < (mid - l + 1))
					__cdmg_inner_iteropt_push(l, mid, (*x).l, 0);
			}
			else if (state == 1) {
				int mid = (l + r) / 2;
				iterstackstate.pop();
				iterstackstate.push(2);
				if ((*x).r != NULL && (*((*x).r)).tol < (r - mid))
					__cdmg_inner_iteropt_push(mid + 1, r, (*x).r, 0);
			}
			__cdmg_inner_iteropt_get(l, r, x, state);
		}
	}
	bool __cdmg_inner_iterator_end() {
		return iterstackstate.size() == 0;
	}
	void __cdmg_inner_iterator_init() {
		__cdmg_inner_iteropt_push(1, max_code_store_size, first_plug, 0);
		__cdmg_inner_iterator_next();
	}
	MESSAGE __cdmg_inner_iterator_get() {
		return code_concrete[iterstackl.top()];
	}
	void __cdmg_inner_iterator_clear() {
		while (iterstackstate.size())__cdmg_inner_iteropt_pop();
	}
	int __cdmg_inner_regist(MESSAGE* tobe_rigisted, MESSAGE_SIMPLIFIED* x, int l, int r) {
		if (l == r) {
			if (code_concrete.size() == l)
				code_concrete.push_back(*tobe_rigisted);
			else code_concrete[l] = *tobe_rigisted;
			(*x).tol = 0;
			codesize++;
			return l;
		}
		int mid = (l + r) / 2;
		if ((*x).l == NULL) (*x).l = new MESSAGE_SIMPLIFIED(mid + 1 - l);
		if ((*x).r == NULL) (*x).r = new MESSAGE_SIMPLIFIED(r - mid);
		int ans;
		if ((*((*x).l)).tol > 0)ans = __cdmg_inner_regist(tobe_rigisted, (*x).l, l, mid);
		else ans = __cdmg_inner_regist(tobe_rigisted, (*x).r, mid + 1, r);
		(*x).tol = (*(*x).l).tol + (*(*x).r).tol;
		return ans;
	}
	bool __cdmg_inner_out(int _pos, MESSAGE_SIMPLIFIED* x, int l, int r) {
		if (l == r) {
			if ((*x).tol == 1) {
#if OpenLog
				LogOut << "Warming : CodeManager - Out - CODE has been logout" << endl;
				LogOut << "Warming : CodeManager - Out - Refuse to OUT" << endl;
				LogOut << "LogTime : " << clock() << endl;
#endif // OpenLog
#if CodeManagerOpenMessagebox
				MessageBox(handle_window, "Warming", "Warming : CodeManager\nFunction : Out\nConcrete : CODE has been logout\n\tRefuse to OUT", MB_OK);
#endif
				return 0;
			}
			(*x).tol = 1;
			codesize--;
			return 1;
		}
		int mid = (l + r) / 2;
		if ((*x).l == NULL) (*x).l = new MESSAGE_SIMPLIFIED(mid + 1 - l);
		if ((*x).r == NULL) (*x).r = new MESSAGE_SIMPLIFIED(r - mid);
		bool ans;
		if (_pos <= mid)ans = __cdmg_inner_out(_pos, (*x).l, l, mid);
		else ans = __cdmg_inner_out(_pos, (*x).r, mid + 1, r);
		(*x).tol = (*(*x).l).tol + (*(*x).r).tol;
		return ans;
	}
	void __cdmg_inner_clearall(MESSAGE_SIMPLIFIED* x, int l, int r) {
		if (l == r) {
			(*x).tol = 1;
			return;
		}
		int mid = (l + r);
		if (l != NULL)__cdmg_inner_clearall((*x).l, l, mid);
		if (r != NULL)__cdmg_inner_clearall((*x).r, mid + 1, r);
		(*x).tol = (r - l + 1);
	}
	// ----------------------------------------------------------------
	// public here
public:
	CODE_MANAGER() :codesize(0) {
		MESSAGE* msg = new MESSAGE;
		EmptyMessage = *msg;
		code_concrete.push_back(*msg);
		delete msg;
		first_plug = new MESSAGE_SIMPLIFIED(max_code_store_size);
#if OpenLog
		if (!LogOut.is_open())LogOut.open("CodeManagerLog.txt");
#endif // OpenLog
	};
	int regist(MESSAGE tobe_rigisted) {
		if (codesize == max_code_store_size) {
#if OpenLog
			LogOut << "Warming : CodeManager - Regist - code size reaches MAX" << endl;
			LogOut << "Warming : CodeManager - Regist - Refuse to RIGIST" << endl;
			LogOut << "LogTime : " << clock() << endl;
#endif // OpenLog
#if CodeManagerOpenMessagebox
			MessageBox(handle_window, "Warming", "Warming : CodeManager\nFunction : Regist\nConcrete : code size reaches MAX\n\tRefuse to RIGIST", MB_ICONWARNING | MB_OK);
#endif
			return -1;
		}
		return __cdmg_inner_regist(&tobe_rigisted, first_plug, 1, max_code_store_size);
	}
	bool out(int code) {
		return __cdmg_inner_out(code, first_plug, 1, max_code_store_size);
	}
	int size() { return codesize; }
	MESSAGE get(int code) {
		if (code > code_concrete.size() - 1) {
#if OpenLog
			LogOut << "Error : CodeManager - Get - invalid requirement" << endl;
			LogOut << "Error : CodeManager - Get - code out of vector size" << endl;
			LogOut << "LogTime : " << clock() << endl;
#endif // OpenLog
#if CodeManagerOpenMessagebox
			MessageBox(handle_window, "Error", "Error : CodeManager\nFunction : Get\nConcrete : invalid requirement\n\tcode out of vector size", MB_ICONERROR | MB_OK);
#endif
			return EmptyMessage;
		}
		return code_concrete[code];
	}
	void AllClear() { __cdmg_inner_clearall(); codesize = 0; }
	void iterbegin() { __cdmg_inner_iterator_init(); }
	bool iterend() { return __cdmg_inner_iterator_end(); }
	void iternext() { __cdmg_inner_iterator_next(); }
	MESSAGE iterget() { return __cdmg_inner_iterator_get(); }
	void cleariter() { __cdmg_inner_iterator_clear(); }
};
template <class OBJECTCLASS> class prior_iterator {
private:
	vector<OBJECTCLASS> sequence;
	int it;
public:
	void cleariter() { sequence.clear(); }
	void add(OBJECTCLASS _obj) { sequence.push_back(_obj); }
	template <typename MANAGER_CLASS>void addall(MANAGER_CLASS& _pos) {
		for (_pos.iterbegin(); !_pos.iterend(); _pos.iternext()) {
			add(_pos.iterget());
		}
		_pos.cleariter();
	}
	void iterbegin() {
		sort(sequence.begin(), sequence.end());
		it = 0;
	}
	OBJECTCLASS iterget() { return sequence[it]; }
	bool iterend() { return it == sequence.size(); }
	void iternext() { it++; }
};
union UniType {
	SURF_TYPE surface;
	SPRITE_TYPE sprite;
	FLASH_TYPE flash;
	TEXT_TYPE text;

};
class UniWindow{
	MESSAGECODE MainBottom;
	vector<MESSAGECODE>Saving;
	vector<MESSAGE_SURF_STATE>show_state;
	vector<MESSAGE_SURF_STATE>hide_state;
	void add(MESSAGECODE to_be_saved) { Saving.push_back(to_be_saved); }
	void del(MESSAGECODE to_be_del) { Saving.erase(find(Saving.begin(), Saving.end(), to_be_del)); }
	void insert(MESSAGECODE to_be_insert, int pos) { Saving.insert(Saving.begin() + pos, to_be_insert); }
	void clear() { Saving.clear(); }

};