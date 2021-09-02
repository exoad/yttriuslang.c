const { MessageEmbed } = require("discord.js");
const config = require("../../configs/token.json");
const chnl = require("../../configs/chnl.json");
const resource = require("../../configs/resource.json");
const superagent = require("superagent");
const colors = require("../../configs/colors.json");
const ready = require("../../configs/ready.json");
module.exports = {
    config: {
        name: `bot`,
        category: "",
        description: "",
        aliases: [``],
    },
    run: async(bot, message, args) => {
        try {
            const embed = new MessageEmbed()
                .setTitle("Science Bot")
                .setDescription(
                    "Science Bot is a discord bot created by `exoad#4166`. It is a bot that incorporates science as its main focus."
                )
                .addField("Status", ready.status)
                .addField(
                    "Bot Package",
                    "[Using a C++ Discord Bot API](https://github.com/yourWaifu/sleepy-discord)"
                )
                .addField(
                    "APIs & Dependencies",
                    "1. [NASA Open APIs](https://api.nasa.gov/)\n2. [LaTeX](https://github.com/latex3/)\n3. [Periodic Table Source](http://www.cplusplus.com/forum/general/89585/)\n4. [Wikipedia Public API](https://www.mediawiki.org/wiki/API:Main_page)"
                )
                .addField("Topics Covered", "`Chemistry, Earth Science, Astronomy`")
                .addField(
                    "Extra notes",
                    "Use `" + config.prefix + "support` for more info"
                );

            message.channel.send({ embed });
        } catch (err) {
            const embed = new MessageEmbed()
                .setTitle("Whoops, looks like something went wrong!")
                .setThumbnail(resource.aw_snap)
                .setDescription(
                    "Use `" +
                    config.prefix +
                    "help` for a list of avaliable commands or use `" +
                    config.prefix +
                    "support` to join the support server!"
                )
                .setFooter("Still facing issues? Join the support server!");
            message.channel.send({ embed }).then((m) => {
                m.delete({ timeout: 5000 });
            });
        }
    },
};