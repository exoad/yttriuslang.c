const { MessageEmbed } = require("discord.js");
const superagent = require("superagent");
const resource = require("../../../../configs/resource.json");
const config = require("../../../../configs/token.json");
module.exports = {
    config: {
        name: `fact`,
        category: "",
        description: "",
        aliases: [`coolfact`, `randomfact`, `dumbfact`],
    },
    run: async(bot, message, args) => {
        try {
            const { body } = await superagent.get("https://nekos.life/api/v2/fact");

            const embed = new MessageEmbed()
                .setTitle("Random Facts")
                .setDescription(body.fact)
                .setColor("RANDOM")
                .setTimestamp();

            message.channel.send({ embed });
        } catch (err) {
            const embed = new MessageEmbed()
                .setTitle(
                    "Whoops, looks like the requested command doesn't exist (yet)!"
                )
                .setThumbnail(resource.aw_snap)
                .setDescription(
                    "Use `" + config.prefix + "help` for a list of avaliable commands!"
                )
                .setFooter("Still facing issues? Join the support server!");
            message.channel.send({ embed }).then((m) => {
                m.delete({ timeout: 5000 });
            });
        }
    },
};