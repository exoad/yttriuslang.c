const { MessageEmbed } = require("discord.js");
const config = require("../../../configs/token.json");
const resource = require("../../../configs/resource.json");
const token = require("../../../configs/token.json");
module.exports = {
    config: {
        name: `invite`,
        category: "",
        description: "",
        aliases: [`invitebot`],
    },
    run: async(bot, message) => {
        try {
            const embed = new MessageEmbed()
                .setTitle("Invite Sci-Bot to your servers")
                .setDescription(
                    "Wanna invite `Sci-Bot` to your own server(s)? Check the links below!"
                )
                .addField("Invite Link #1 (Admin)", "[Admin](" + token.perm1 + ")")
                .addField(
                    "Invite Link #2 (Primary Permissions)",
                    "[Primary Permissions](" + token.perm2 + ")"
                )
                .addField(
                    "Invite Link #3 (Limited Permissions)",
                    "[Limited Permissions](" + token.perm3 + ")"
                )
                .setFooter("Thanks for inviting the bot!");

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