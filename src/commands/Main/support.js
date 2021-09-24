const { MessageEmbed } = require("discord.js");
const config = require("../../../configs/token.json");
const resource = require("../../../configs/resource.json");

module.exports = {
    config: {
        name: `support`,
        category: "",
        description: "",
        aliases: [`supportserver`],
    },
    run: async(bot, message) => {
        try {
            const embed = new MessageEmbed()
                .setTitle("Support")
                .setDescription(
                    "Have questions regarding usage of the bot? Come join the discord!"
                )
                .addField(
                    "Main Discord Support Server",
                    "[Invite Link](" + config.server_invite1 + ")"
                )
                .addField(
                    "Backup Invite Link",
                    "[Backup Link](" + config.backup_server_invite + ")"
                );

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