#include <iostream>
#include <string>

#include "aoch/AOCSolutionTypes.hpp"
#include "aoch/string.hpp"


/*
 * @TODO: refactor this entire mess...
 *
 */
struct Wizzard;

struct Boss {
    int hp = 100;
    int dmg = 10;

    int poison_timer = 0;

    void advance_turn() {

        if (poison_timer) {
            hp -= 3;
            poison_timer--;
        }
    }

    void attack(Wizzard& w);

    bool is_dead() {
        if (hp <= 0) return true;
        else return false;
    }
};

struct Wizzard {
    int hp = 50;
    int mana = 500;
    int def = 0;

    int shield_timer = 0;
    int recharge_timer = 0;

    const int SHIELD_DEF_INC = 7;

    void advance_turn() {

        if (shield_timer) {
            shield_timer--;

            if (shield_timer == 0) {
                def -= SHIELD_DEF_INC;
            }
        }

        if (recharge_timer) {
            mana += 101;
            recharge_timer--;
        }
    }

    bool can_cast_shield() {
        if (mana < 113 || shield_timer) return false;
        else return true;
    }

    bool cast_shield() {

        if (can_cast_shield()) {
            mana -= 113;
            shield_timer = 6;
            def += SHIELD_DEF_INC;

            return true;
        } else {
            return false;
        }
    }

    bool can_cast_magic_missle() {
        if (mana < 53) return false;
        else return true;
    }

    bool cast_magic_missle(Boss& b) {

        if (can_cast_magic_missle()) {
            mana -= 53;
            b.hp -= 4;
            return true;
        } else {
            return false;
        }
    }

    bool can_cast_drain() {
        if (mana < 73) return false;
        else return true;
    }

    bool cast_drain(Boss& b) {
        if (can_cast_drain()) {

            mana -= 73;
            hp += 2;
            b.hp -= 2;
            return true;

        } else {
            return false;
        }
    }

    bool can_cast_recharge() {
        if (mana < 229 || recharge_timer) return false;
        else return true;
    }

    bool cast_recharge() {

        if (can_cast_recharge()) {

            mana -= 229;
            recharge_timer = 5;
            return true;

        } else {
            return false;
        }

    }

    bool can_cast_poison(Boss& b) {
        if (mana < 173 || b.poison_timer) return false;
        else return true;
    }

    bool cast_poison(Boss& b) {

        if (can_cast_poison(b)) {

            mana -= 173;
            b.poison_timer = 6;
            return true;

        } else {
            return false;
        }
    }

    bool is_dead() {
        if (hp <= 0) return true;
        else return false;
    }
};

void Boss::attack(Wizzard& w) {
    w.hp -= std::max(1, dmg - w.def);
}

int calc_min_mana_used_to_win(Wizzard w, Boss b, int used_mana = 0) {

    w.advance_turn();
    b.advance_turn();

    if (b.is_dead()) return used_mana;


    int best_used_mana = -1;

    if (w.can_cast_magic_missle()) {
        Wizzard wc = w;
        Boss bc = b;

        wc.cast_magic_missle(bc);

        if (bc.is_dead()) return used_mana + 53;

        wc.advance_turn();
        bc.advance_turn();

        if (bc.is_dead()) return used_mana + 53;

        bc.attack(wc);

        if (wc.is_dead()) return -1;

        int ret = calc_min_mana_used_to_win(wc, bc, used_mana + 53);

        if (ret >= 0) {
            if (best_used_mana < 0) best_used_mana = ret;
            else best_used_mana = std::min(best_used_mana, ret);
        }
    }

    if (w.can_cast_shield()) {
        Wizzard wc = w;
        Boss bc = b;

        wc.cast_shield();

        wc.advance_turn();
        bc.advance_turn();

        if (bc.is_dead()) return used_mana + 113;

        bc.attack(wc);

        if (wc.is_dead()) return -1;

        int ret = calc_min_mana_used_to_win(wc, bc, used_mana + 113);

        if (ret >= 0) {
            if (best_used_mana < 0) best_used_mana = ret;
            else best_used_mana = std::min(best_used_mana, ret);
        }
    }

    if (w.can_cast_drain()) {
        Wizzard wc = w;
        Boss bc = b;

        wc.cast_drain(bc);

        if (bc.is_dead()) return used_mana + 73;

        wc.advance_turn();
        bc.advance_turn();

        if (bc.is_dead()) return used_mana + 73;

        bc.attack(wc);

        if (wc.is_dead()) return -1;

        int ret = calc_min_mana_used_to_win(wc, bc, used_mana + 73);

        if (ret >= 0) {
            if (best_used_mana < 0) best_used_mana = ret;
            else best_used_mana = std::min(best_used_mana, ret);
        }

    }

    if (w.can_cast_recharge()) {
        Wizzard wc = w;
        Boss bc = b;

        wc.cast_recharge();

        wc.advance_turn();
        bc.advance_turn();

        if (bc.is_dead()) return used_mana + 229;

        bc.attack(wc);

        if (wc.is_dead()) return -1;

        int ret = calc_min_mana_used_to_win(wc, bc, used_mana + 229);

        if (ret >= 0) {
            if (best_used_mana < 0) best_used_mana = ret;
            else best_used_mana = std::min(best_used_mana, ret);
        }
    }

    if (w.can_cast_poison(b)) {
        Wizzard wc = w;
        Boss bc = b;

        wc.cast_poison(bc);

        wc.advance_turn();
        bc.advance_turn();

        if (bc.is_dead()) return used_mana + 173;

        bc.attack(wc);

        if (wc.is_dead()) return -1;

        int ret = calc_min_mana_used_to_win(wc, bc, used_mana + 173);

        if (ret >= 0) {
            if (best_used_mana < 0) best_used_mana = ret;
            else best_used_mana = std::min(best_used_mana, ret);
        }
    }

    return best_used_mana;
}

int calc_min_mana_used_to_win2(Wizzard w, Boss b, int used_mana = 0) {

    w.hp -= 1;

    if (w.is_dead()) return -1;

    w.advance_turn();
    b.advance_turn();

    if (b.is_dead()) return used_mana;


    int best_used_mana = -1;

    if (w.can_cast_magic_missle()) {
        Wizzard wc = w;
        Boss bc = b;

        wc.cast_magic_missle(bc);

        if (bc.is_dead()) return used_mana + 53;

        wc.advance_turn();
        bc.advance_turn();

        if (bc.is_dead()) return used_mana + 53;

        bc.attack(wc);

        if (wc.is_dead()) return -1;

        int ret = calc_min_mana_used_to_win2(wc, bc, used_mana + 53);

        if (ret >= 0) {
            if (best_used_mana < 0) best_used_mana = ret;
            else best_used_mana = std::min(best_used_mana, ret);
        }
    }

    if (w.can_cast_shield()) {
        Wizzard wc = w;
        Boss bc = b;

        wc.cast_shield();

        wc.advance_turn();
        bc.advance_turn();

        if (bc.is_dead()) return used_mana + 113;

        bc.attack(wc);

        if (wc.is_dead()) return -1;

        int ret = calc_min_mana_used_to_win2(wc, bc, used_mana + 113);

        if (ret >= 0) {
            if (best_used_mana < 0) best_used_mana = ret;
            else best_used_mana = std::min(best_used_mana, ret);
        }
    }

    if (w.can_cast_drain()) {
        Wizzard wc = w;
        Boss bc = b;

        wc.cast_drain(bc);

        if (bc.is_dead()) return used_mana + 73;

        wc.advance_turn();
        bc.advance_turn();

        if (bc.is_dead()) return used_mana + 73;

        bc.attack(wc);

        if (wc.is_dead()) return -1;

        int ret = calc_min_mana_used_to_win2(wc, bc, used_mana + 73);

        if (ret >= 0) {
            if (best_used_mana < 0) best_used_mana = ret;
            else best_used_mana = std::min(best_used_mana, ret);
        }

    }

    if (w.can_cast_recharge()) {
        Wizzard wc = w;
        Boss bc = b;

        wc.cast_recharge();

        wc.advance_turn();
        bc.advance_turn();

        if (bc.is_dead()) return used_mana + 229;

        bc.attack(wc);

        if (wc.is_dead()) return -1;

        int ret = calc_min_mana_used_to_win2(wc, bc, used_mana + 229);

        if (ret >= 0) {
            if (best_used_mana < 0) best_used_mana = ret;
            else best_used_mana = std::min(best_used_mana, ret);
        }
    }

    if (w.can_cast_poison(b)) {
        Wizzard wc = w;
        Boss bc = b;

        wc.cast_poison(bc);

        wc.advance_turn();
        bc.advance_turn();

        if (bc.is_dead()) return used_mana + 173;

        bc.attack(wc);

        if (wc.is_dead()) return -1;

        int ret = calc_min_mana_used_to_win2(wc, bc, used_mana + 173);

        if (ret >= 0) {
            if (best_used_mana < 0) best_used_mana = ret;
            else best_used_mana = std::min(best_used_mana, ret);
        }
    }

    return best_used_mana;
}

aoch::Result solve_day22(aoch::Input& in) {
    aoch::Result r;

    Boss b;
    b.hp = std::stoi(aoch::split(in[0])[2]);
    b.dmg = std::stoi(aoch::split(in[1])[1]);

    Wizzard w;

    int mana_used = calc_min_mana_used_to_win(w, b);
    r.part1 = std::to_string(mana_used);

    int mana_used2 = calc_min_mana_used_to_win2(w, b);
    r.part2 = std::to_string(mana_used2);

    return r;
}
