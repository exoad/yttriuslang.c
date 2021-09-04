const molar = new Array();
molar["H"] = 1.00794;
molar["He"] = 4.0026;
molar["Li"] = 6.941;
molar["Be"] = 9.01218;
molar["B"] = 10.811;
molar["C"] = 12.011;
molar["N"] = 14.0067;
molar["O"] = 15.9994;
molar["F"] = 18.9984;
molar["Ne"] = 20.1797;
molar["Na"] = 22.98977;
molar["Mg"] = 24.305;
molar["Al"] = 26.98154;
molar["Si"] = 28.0855;
molar["P"] = 30.97376;
molar["S"] = 32.066;
molar["Cl"] = 35.4527;
molar["Ar"] = 39.948;
molar["K"] = 39.0983;
molar["Ca"] = 40.078;
molar["Sc"] = 44.9559;
molar["Ti"] = 47.88;
molar["V"] = 50.9415;
molar["Cr"] = 51.996;
molar["Mn"] = 54.938;
molar["Fe"] = 55.847;
molar["Co"] = 58.9332;
molar["Ni"] = 58.6934;
molar["Cu"] = 63.546;
molar["Zn"] = 65.39;
molar["Ga"] = 69.723;
molar["Ge"] = 72.61;
molar["As"] = 74.9216;
molar["Se"] = 78.96;
molar["Br"] = 79.904;
molar["Kr"] = 83.8;
molar["Rb"] = 85.4678;
molar["Sr"] = 87.62;
molar["Y"] = 88.9059;
molar["Zr"] = 91.224;
molar["Nb"] = 92.9064;
molar["Mo"] = 95.94;
molar["Tc"] = 98;
molar["Ru"] = 101.07;
molar["Rh"] = 102.9055;
molar["Pd"] = 106.42;
molar["Ag"] = 107.868;
molar["Cd"] = 112.41;
molar["In"] = 114.82;
molar["Sn"] = 118.71;
molar["Sb"] = 121.757;
molar["Te"] = 127.6;
molar["I"] = 126.9045;
molar["Xe"] = 131.29;
molar["Cs"] = 132.9054;
molar["Ba"] = 137.33;
molar["La"] = 138.9055;
molar["Ce"] = 140.12;
molar["Pr"] = 140.9077;
molar["Nd"] = 144.24;
molar["Pm"] = 145;
molar["Sm"] = 150.36;
molar["Eu"] = 151.965;
molar["Gd"] = 157.25;
molar["Tb"] = 158.9253;
molar["Dy"] = 162.5;
molar["Ho"] = 164.9303;
molar["Er"] = 167.26;
molar["Tm"] = 168.9342;
molar["Yb"] = 173.04;
molar["Lu"] = 174.967;
molar["Hf"] = 178.49;
molar["Ta"] = 180.9479;
molar["W"] = 183.85;
molar["Re"] = 186.207;
molar["Os"] = 190.2;
molar["Ir"] = 192.22;
molar["Pt"] = 195.08;
molar["Au"] = 196.9665;
molar["Hg"] = 200.59;
molar["Tl"] = 204.383;
molar["Pb"] = 207.2;
molar["Bi"] = 208.9804;
molar["Po"] = 209;
molar["At"] = 210;
molar["Rn"] = 222;
molar["Fr"] = 223;
molar["Ra"] = 226.0254;
molar["Ac"] = 227;
molar["Th"] = 232.0381;
molar["Pa"] = 231.0359;
molar["U"] = 238.029;
molar["Np"] = 237.0482;
molar["Pu"] = 244;
molar["Am"] = 243;
molar["Cm"] = 247;
molar["Bk"] = 247;
molar["Cf"] = 251;
molar["Es"] = 252;
molar["Fm"] = 257;
molar["Md"] = 258;
molar["No"] = 259;
molar["Lr"] = 266;
molar["Rf"] = 267;
molar["Db"] = 268;
molar["Sg"] = 269;
molar["Bh"] = 270;
molar["Hs"] = 269;
molar["Mt"] = 278;
molar["Ds"] = 281;
molar["Rg"] = 282;
molar["Cn"] = 285;
molar["Nh"] = 286;
molar["Fl"] = 289;
molar["Mc"] = 290;
molar["Lv"] = 293;
molar["Ts"] = 294;
molar["Og"] = 294;
molar["Me"] = 15.035;
molar["Et"] = 29.062;
molar["Pr"] = 43.089;
molar["Bu"] = 57.115;
molar["Ts"] = 155.197;
molar["Aq"] = 18.015;
molar["Ph"] = 77.106;
// molar["Ac"]= 43.045;
const uppers = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
const lowers = 'abcdefghijklmnopqrstuvwxyz';
const nums = '1234567890';

/**
 * Reads all atoms in the formula, adds up all their molar masses.
 * @param {String} formula 
 * @returns {JSON} Object containing an array of atoms found, and a number with the mass calculated.
 */
function calc(formula) {
    let mass = 0;
    let atoms = interpFormula(formula);
    atoms.forEach(atom => {
        mass += molar[atom];
    });
    mass = mass.toPrecision(6);
    return({atoms, mass});
}

/**
 * Reads the formula and returns an array with all the atoms.
 * @param {String} formula 
 * @returns {Arrray} an array with all found atoms in it, as strings.
 */
function interpFormula(formula) {
    let atoms = [];

    for (let i = 0; i < formula.length; i++) {
        let c = formula.charAt(i);
        if (uppers.includes(c)) {
            let nc = formula.charAt(i + 1);
            if (uppers.includes(nc) || nums.includes(nc) || nc == '(') {
                atoms.push(c);
            } else if (lowers.includes(nc)) {
                atoms.push('' + c + nc);
            }
        } else if (nums.includes(c) && i > 0) {
            let number = getNumberStartingAt(i, formula);

            let atomPos = atoms.length - 1;
            for (let j = 1; j < number; j++) {
                atoms.push(atoms[atomPos]);
            }

            i += ('' + number).length - 1;
        } else if (c == '(') {
            let start = i;
            let size = 0;
            do {
                ++size;
            } while (formula.charAt(start + size) != ')' && start + size < formula.length);

            let subformula = formula.substring(start + 1, start + size);
            let innerAtoms = interpFormula(subformula);
            i += size;

            if (nums.includes(formula.charAt(start + size + 1))) {
                let number = getNumberStartingAt(i + 1, formula);
                i += ('' + number).length;
                do {
                    atoms = atoms.concat(innerAtoms);
                } while (1 < number--);
            } else {
                atoms = atoms.concat(innerAtoms);
            }
        }
    }

    return atoms;
}

/**
 * Reads a number contained inside string 'formula', starting at position 'i'.
 * Used in 'interpFormula' to read numbers.
 * Checks the characters one by one and stops when the character is not a number.
 * Returns the substring found to contain consecutive numbers starting from position 'i'.
 * @param {Number} i 
 * @param {String} formula
 * @returns {Number} Number found inside formula, starting at 'i' and ending somewhere else in the string.
 */
function getNumberStartingAt(i, formula) {
    let checkNext = true;
    let pos = i;
    do {
        if (!nums.includes(formula.charAt(pos))) {
            checkNext = false;
        } else {
            pos++;
        }
    } while (checkNext && pos < formula.length);

    return parseInt(formula.substring(i, pos + 1));
}

module.exports = {
    calc
}