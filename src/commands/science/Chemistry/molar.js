const { MessageEmbed } = require("discord.js");
const config = require("../../../../configs/token.json");
const resource = require("../../../../configs/resource.json");
const colors = require("../../../../configs/colors.json");
const molarcalc = require("molarcalc");
module.exports = {
    config: {
        name: `molar`,
        category: "",
        description: "",
        aliases: [``],
    },
    run: async(bot, message, args) => {
        try {
            let mole = args[0];
            if (!mole || mole == undefined) {
                const embed = new MessageEmbed()
                    .setThumbnail(resource.aw_snap)
                    .setTitle("Oops, looks like your missing some parameters")
                    .addField(
                        "Proper Usage",
                        "`" + config.prefix + "molar {formula/compound}`"
                    )
                    .addField(
                        "Description",
                        "Input a chemical formula to get it's molar mass"
                    )
                    .addField(
                        "Exclusions (Unreadable)",
                        "1. `Tri` & `acetyl`\n2. `Ac` for `acetyl` (Write as CH3CO, etc.)\n3. Spaces are not allowed\n4. Make sure each letter is capitalzed correctly"
                    )
                    .addField("Example Usage", "```" + config.prefix + "molar H2O```")
                    .setFooter("Have questions? Join the support server!")
                    .setColor(colors.error);

                message.channel.send({ embed });
            } else {
                const reg = JSON.parse(JSON.stringify(molarcalc.calc(mole)));
                const embed = new MessageEmbed()
                    .setTitle("Molar Mass for: " + reg)
                    .addField("Atoms Contents:", reg.atoms)
                    .addField("Molar Mass:", reg.mass);

                message.channel.send({ embed });
            }
        } catch (err) {
            const embed = new MessageEmbed()
                .setTitle("Whoops, looks like something went wrong!")
                .setThumbnail(resource.aw_snap)
                .setDescription(
                    "__**Check the Following:**__\n1. Is letters in your formula capitalized correctly?\n2. Does it contain anything from the exclusion's list?\n3. Make sure it is a valid formula"
                )
                .setFooter("Still facing issues? Join the support server!");
            message.channel.send({ embed }).then((m) => {
                m.delete({ timeout: 5000 });
            });
        }
    },
};