#ifndef _MAIN_CPP_
#define _MAIN_CPP_

#include <yvals_core.h>

#if _STL_COMPILER_PREPROCESSOR

#if !_HAS_CXX23
#error _CXX23_REQUIRED_
#endif

#pragma warning(push)
#pragma warning(disable : 4100 4324)
#pragma pack(push, _CRT_PACKING)
#pragma push_macro("new")
#undef new

#include <chrono>
#include <conio.h>
#include <expected>
#include <print>
#include <thread>

#define _STD ::std::
#define _CSTD ::
#define _NODISCARD [[nodiscard]]

_STD_BEGIN

struct alignas(32) _Pong_state
{
    int _Ball_x;
    int _Ball_y;
    int _Ball_vx;
    int _Ball_vy;
    int _P1_y;
    int _P2_y;
    int _P1_score;
    int _P2_score;
    int _Width;
    int _Height;
    int _Paddle_len;
};

enum class _Tile_type : char
{
    _Empty = ' ',
    _Paddle = '#',
    _Ball = 'O',
    _Wall = '-'
};

_STD_END

extern "C"
{
    void __stdcall _Update_pong_physics_asm(_STD _Pong_state *_State) noexcept;
    void __stdcall _Clear_board_asm(void *_Dest, _STD size_t _Qwords) noexcept;
}

_STD_BEGIN

template <size_t _Width, size_t _Height> class _Pong_engine
{
  private:
    alignas(64) _Tile_type _Board_data[_Width * _Height];
    _Pong_state _State;
    bool _Is_running = true;

  public:
    constexpr _Pong_engine() noexcept
    {
        _State._Ball_x = static_cast<int>(_Width / 2);
        _State._Ball_y = static_cast<int>(_Height / 2);
        _State._Ball_vx = -1;
        _State._Ball_vy = 1;
        _State._P1_y = static_cast<int>(_Height / 2);
        _State._P2_y = static_cast<int>(_Height / 2);
        _State._P1_score = 0;
        _State._P2_score = 0;
        _State._Width = static_cast<int>(_Width);
        _State._Height = static_cast<int>(_Height);
        _State._Paddle_len = 4;
    }

    constexpr ~_Pong_engine() noexcept = default;
    constexpr _Pong_engine(const _Pong_engine &) noexcept = default;
    constexpr _Pong_engine &operator=(const _Pong_engine &) noexcept = default;

    void _Run() noexcept
    {
        _STD print("\x1b[2J\x1b[H");
        _STD println("MASM x64 Pong");
        static_cast<void>(_CSTD _getch());

        using namespace _STD chrono_literals;
        auto _Last_tick = _STD chrono::steady_clock::now();

        while (_Is_running)
        {
            if (_CSTD _kbhit())
            {
                const char _Input = static_cast<char>(_CSTD _getch());
                if ((_Input == 'w' || _Input == 'W') && _State._P1_y > 0)
                {
                    _State._P1_y--;
                }
                else if ((_Input == 's' || _Input == 'S') &&
                         _State._P1_y < static_cast<int>(_Height) - _State._Paddle_len)
                {
                    _State._P1_y++;
                }
                else if (_Input == 'q' || _Input == 'Q')
                {
                    break;
                }
            }

            auto _Current_time = _STD chrono::steady_clock::now();
            if (_Current_time - _Last_tick >= 30ms)
            {
                _Last_tick = _Current_time;

                _Update_pong_physics_asm(&_State);

                if (_State._P1_score >= 5 || _State._P2_score >= 5)
                {
                    _Is_running = false;
                }
                else
                {
                    _Render_frame();
                }
            }
            _STD this_thread::sleep_for(5ms);
        }
    }

  protected:
    template <class _Self> void _Render_frame(this const _Self &_Self_obj) noexcept
    {
        constexpr size_t _Total_bytes = _Width * _Height * sizeof(_Tile_type);
        constexpr size_t _Qwords = _Total_bytes / 8;

        __assume(_Total_bytes % 8 == 0);

        _Clear_board_asm(const_cast<_Tile_type *>(_Self_obj._Board_data), _Qwords);

        for (int _I = 0; _I < _Self_obj._State._Paddle_len; ++_I)
        {
            const_cast<_Tile_type *>(_Self_obj._Board_data)[(_Self_obj._State._P1_y + _I) * _Width + 1] =
                _Tile_type::_Paddle;
            const_cast<_Tile_type *>(_Self_obj._Board_data)[(_Self_obj._State._P2_y + _I) * _Width + (_Width - 2)] =
                _Tile_type::_Paddle;
        }

        const int _Bx = _Self_obj._State._Ball_x;
        const int _By = _Self_obj._State._Ball_y;
        if (_Bx >= 0 && _Bx < static_cast<int>(_Width) && _By >= 0 && _By < static_cast<int>(_Height))
        {
            const_cast<_Tile_type *>(_Self_obj._Board_data)[_By * _Width + _Bx] = _Tile_type::_Ball;
        }

        _STD print("\x1b[H");
        _STD println("MASM x64 Pong");
        _STD println("P1: {}  |  P2: {}", _Self_obj._State._P1_score, _Self_obj._State._P2_score);

        _STD print("+");
        for (size_t _C = 0; _C < _Width + 2; ++_C)
            _STD print("-");
        _STD println("");

        for (size_t _R = 0; _R < _Height; ++_R)
        {
            _STD print("|");
            for (size_t _C = 0; _C < _Width; ++_C)
            {
                char _Char = static_cast<char>(_Self_obj._Board_data[_R * _Width + _C]);
                _STD print("{}", _Char == 0 ? ' ' : _Char);
            }
            _STD println("|");
        }

        _STD print("+");
        for (size_t _C = 0; _C < _Width + 2; ++_C)
            _STD print("-");
        _STD println("");
    }
};

_STD_END

int main() noexcept
{
    ::std::_Pong_engine<64, 20> _Game_instance;
    _Game_instance._Run();
    return 0;
}

#pragma pop_macro("new")
#pragma pack(pop)
#pragma warning(pop)

#endif
#endif
