const { MessageEmbed } = require("discord.js");
const config = require("../../../../configs/token.json");
const chnl = require("../../../../configs/chnl.json");
const resource = require("../../../../configs/resource.json");
const chemicaltools = require("chemicaltools");
const colors = require("../../../../configs/colors.json");
const npt = require("node-periodic-table");
module.exports = {
    config: {
        name: `element`,
        category: "",
        description: "",
        aliases: [``],
    },
    run: async(bot, message, args) => {
        try {
            function capStuff(cap) {
                return cap.charAt(0).toUpperCase() + cap.slice(1);
            }
            let toSearch = args[0];
            if (!toSearch || toSearch == undefined) {
                const embed = new MessageEmbed()
                    .setTitle("Oops, looks like you are missing some arguments")
                    .setDescription(
                        "**Usage:** `" + config.prefix + "element {element_name}`\n"
                    )
                    .addField(
                        "Accepted Parameters for [element_name]:",
                        "`element_symbol`, `element_atomic_number`, `element_fullname`"
                    )
                    .setFooter("Have questions? Join the support server!")
                    .setThumbnail(resource.aw_snap)
                    .setColor(colors.error);

                message.channel.send({ embed });
            } else {
                const smth = JSON.stringify(chemicaltools.searchElement(capStuff(toSearch)));
                var body = JSON.parse(smth);
                if (body.iupac == null || !body.iupac || body.iupac == undefined) {
                    const embed = new MessageEmbed()
                        .setTitle("Hmmm, looks like that element doesn't exist (yet)")
                        .setDescription(
                            "**Usage:** `" + config.prefix + "element {element_name}`\n"
                        )
                        .addField(
                            "Accepted Parameters for [element_name]:",
                            "`element_symbol`, `element_atomic_number`, `element_fullname`"
                        )
                        .addField(
                            "Example",
                            "`" +
                            config.prefix +
                            "element Hydrogen`\n**Parameters Taken:** `element_fullname`"
                        )
                        .setFooter("Have questions? Join the support server!")
                        .setThumbnail(resource.aw_snap)
                        .setColor(colors.error);

                    message.channel.send({ embed });
                } else {
                    const oc = JSON.stringify(npt.getBySymbol(body.symbol));
                    const ewww = JSON.parse(oc);
                    const embed = new MessageEmbed()
                        .setTitle(body.iupac)
                        .setDescription(
                            `\n**Symbol:** ${body.symbol}\n**Atomic Number:** ${body.number}\n**Atomic Mass: ** ${body.mass}\n**Origin:** ${body.origin}\n**Appearance:** ${ewww.appearance}\n**Boil (K):** ${ewww.boil}\n**Category:** ${ewww.category}\n**Color (If Any):** ${ewww.color}\n**Density:** ${ewww.density}\n**Discoverer:** ${ewww.discovered_by}\n**Melting Point (K):** ${ewww.melt}\n**Molar Heat:** ${ewww.molar_heat}\n**Named By:** ${ewww.named_by}\n**Main State:** ${ewww.phase}\n**Course Description:** ${ewww.summary}\n**Position on Periodic Table (X, Y):** (${ewww.xpos}, ${ewww.ypos})\n**Shells:** ${ewww.shells}\n**Electron Config.:** ${ewww.electron_configuration}\n**Electron Affinity:** ${ewww.electron_affinity}\n**Electronegativity Pauling:** ${ewww.electronegativity_pauling}\n**Ionization Energies:** \`\`\`${ewww.ionization_energies}\`\`\``
                        )
                        .setThumbnail(body.url)
                        .setFooter(`Requested by ${message.author.username}`)
                        .setColor("RANDOM");

                    message.channel.send({ embed });
                }
            }
        } catch (err) {
            bot.channels.cache.get(chnl.errors).send(`${err.stack}`);
            const embed = new MessageEmbed()
                .setTitle("Hmmm, looks like that element doesn't exist (yet)")
                .setDescription(
                    "**Usage:** `" + config.prefix + "element {element_name}`\n"
                )
                .addField(
                    "Accepted Parameters for [element_name]:",
                    "`element_symbol`, `element_atomic_number`, `element_fullname`"
                )
                .addField(
                    "Example",
                    "`" +
                    config.prefix +
                    "element Hydrogen`\n**Parameters Taken:** `element_fullname`"
                )
                .setFooter("Have questions? Join the support server!")
                .setThumbnail(resource.aw_snap)
                .setColor(colors.error);

            message.channel.send({ embed }).then((m) => {
                m.delete({ timeout: 10000 });
            });
        }
    },
};

function stringify(arg0) {
    throw new Error("Function not implemented.");
}
